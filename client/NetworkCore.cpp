
#include "Game.h"
#include "Player.h"
#include "NetworkPacket.h"
#include "Crossroad.h"
#include "NetworkCore.h"

#include "ui_infoboxform.h"

NetworkCore::NetworkCore(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    server = new QTcpServer();

    // create infobox
    infoBoxDialog = new QDialog();
    infoBoxDialog->hide();
    infoBox.setupUi(infoBoxDialog);
    infoBoxDialog->setWindowTitle("Network Status");
    connect(infoBox.buttonCancel, SIGNAL(clicked()),
        this, SLOT(cancelConnect()));
    connect(infoBoxDialog, SIGNAL(finished(int)),
        this, SLOT(cancelConnect()));

    connectTimer.setInterval(5000);
    connectTimer.setSingleShot(false);
    connect(&connectTimer, SIGNAL(timeout()), this, SLOT(retryConnect()));
}

NetworkCore::~NetworkCore()
{
    delete infoBoxDialog;
    delete socket;
    delete server;
}

bool NetworkCore::startServer(uint port)
{
    if(getIsServer()) return true;

    disconnectAll();
    qDebug() << "Starting up server to listen at port" << port;
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
    return server->listen(QHostAddress::Any, port);
}

bool NetworkCore::connectToServer(QString host, uint p)
{
    Q_ASSERT(!connectTimer.isActive());

    qDebug() << "Connecting to" << host << "on port" << p;

    disconnectAll();
    setupSocket(socket);
    infoBoxDialog->show();
    hostName = host;
    port = p;
    connectAttempts = 0;
    retryConnect();

    return true;
}

void NetworkCore::retryConnect()
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        connectTimer.stop();
        infoBoxDialog->close();
        return;
    }

    socket->abort();
    connectAttempts++;

    if(connectAttempts > 5)
    {
        connectTimer.stop();
        QMessageBox::critical(infoBoxDialog, "Error",
            "Host could not be contacted!");
        infoBoxDialog->close();
        return;
    }

    infoBox.label->setText(QString("Connecting to %1:%2... (attempt %3/5)")
        .arg(hostName).arg(port).arg(connectAttempts));

    qDebug() << "Trying to connect to" << hostName << ":" <<
        port << "for the" << connectAttempts << "time";

    socket->connectToHost(hostName, port);

    if(!connectTimer.isActive()) connectTimer.start();
}

void NetworkCore::cancelConnect()
{
    if(socket->state() != QAbstractSocket::ConnectedState)
    {
        connectTimer.stop();
        disconnectAll();
        qDebug() << "Connect attempt canceled.";
    }

    infoBoxDialog->hide();
}

void NetworkCore::connected()
{
    Q_ASSERT(!getIsServer());

    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Connected to" << s->peerAddress();
    connections.append(s);

    connectTimer.stop();
    infoBoxDialog->close();

    // join the game
    GAME->getRules()->executeRule("ruleJoinGame");
}

void NetworkCore::disconnectSocket(QTcpSocket *s)
{
    qDebug() << "Disconnecting from" << s->peerAddress();
    s->disconnect(this);
    s->close();
    connections.removeAll(s);
    if(players.contains(s)) players.remove(s);
    if(s != socket) s->deleteLater();
}

void NetworkCore::disconnectAll()
{
    if(getIsServer())
    {
        qDebug() << "Stopping game server...";
        server->disconnect(this);
        server->close();
    }

    disconnectSocket(socket);

    while(!connections.isEmpty())
    {
        QTcpSocket *sock = connections.takeFirst();
        disconnectSocket(sock);
    }
}

void NetworkCore::sendPacket(const NetworkPacket &packet)
{
    QList<QTcpSocket*>::iterator i;

    for(i = connections.begin(); i != connections.end(); ++i)
    {
        sendPacket((QTcpSocket*)*i, packet);
    }
}

void NetworkCore::sendPacket(QTcpSocket *s, const NetworkPacket &packet)
{
    QByteArray block;
    QDataStream data(&block, QIODevice::WriteOnly);
    data.setVersion(QDataStream::Qt_4_7);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);

    data << (quint32)0;
    data << packet;
    data.device()->seek(0);
    data << (quint32)(block.size() - sizeof(quint32));

    s->write(block);
}

void NetworkCore::acceptNewConnection()
{
    QTcpSocket *sock = server->nextPendingConnection();
    if(!sock) return;

    if(GAME->getState() != Game::PreparingState)
    {
        sock->disconnectFromHost();
        qDebug() << "Game not preparing. New connection from"
            << sock->peerAddress() << "denied.";
        return;
    }

    qDebug() << "New connection from" << sock->peerAddress();
    connections.append(sock);

    setupSocket(sock);
}

void NetworkCore::setupSocket(QTcpSocket *s)
{
    s->disconnect(this);
    connect(s, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(s, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(s, SIGNAL(connected()), this, SLOT(connected()));
}

void NetworkCore::connectionClosed()
{
    if(QTcpSocket *s = qobject_cast<QTcpSocket*>(sender()))
    {
        qDebug() << "Disconnected from" << s->peerAddress();
        connections.removeAll(s);

        if(players.contains(s))
        {
            GAME->getRules()->executeRule("ruleDisconnect", players.value(s));
            players.remove(s);
        }

        s->close();
        if(s != socket) s->deleteLater();

        if(!getIsServer())
        {
            // lost connection to server!
            QMessageBox::critical(0, "Disconnected",
                "Lost connection to game server!");
            GAME->setState(Game::DisconnectedState);
            GAME->reset();
        }
    }

    infoBoxDialog->close();
}

void NetworkCore::packetReceived(QTcpSocket *s, NetworkPacket &packet)
{
    // drop the connection if the hosts sent an invalid packet
    if(!packet.getIsValid())
    {
        qDebug() << "Invalid packet received from" << s->peerAddress();
        s->disconnectFromHost();
        return;
    }

    qDebug() << "Received rule" << s->peerAddress() << packet.getRuleName();

    if(getIsServer())
    {
        if(!players.contains(s)) players.insert(s, packet.getPlayer());
        Q_ASSERT(packet.getPlayer() == players.value(s));
    }

    GAME->getRules()->executeRuleFromNetwork(packet);
}

void NetworkCore::dataAvailable()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    QDataStream data(s);
    data.setVersion(QDataStream::Qt_4_7);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);

    while(s->bytesAvailable() > 0)
    {
        quint32 blockSize = 0;

        if(pendingBlockSizes.contains(s))
            blockSize = pendingBlockSizes.value(s);

        if(blockSize == 0)
        {
            if(s->bytesAvailable() < (int)sizeof(quint32)) return;
            data >> blockSize;
        }

        if(s->bytesAvailable() < blockSize)
        {
            qDebug() << "Waiting for" << blockSize << "bytes from remote";
            pendingBlockSizes.insert(s, blockSize);
            return;
        }

        NetworkPacket packet;
        data >> packet;
        packetReceived(s, packet);
        pendingBlockSizes.remove(s);
    }
}

