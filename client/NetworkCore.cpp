
#include "Game.h"
#include "Player.h"
#include "NetworkPacket.h"
#include "Crossroad.h"
#include "NetworkCore.h"

NetworkCore::NetworkCore(QObject *parent) : QObject(parent)
{
    socket = NULL;
    server = new QTcpServer();
}

NetworkCore::~NetworkCore()
{
    if(socket) delete socket;
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

bool NetworkCore::connectToServer(QString host, uint port)
{
    qDebug() << "Connecting to" << host << "on port" << port;
    Q_ASSERT(socket == NULL);
    disconnectAll();
    socket = new QTcpSocket();
    setupSocket(socket);
    socket->connectToHost(host, port);
    return true;
}

void NetworkCore::connected()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Connected to" << s->peerAddress();
    connections.append(s);

    if(!getIsServer())
    {
        // join the game
        GAME->getRules()->executeRule("ruleJoinGame");
    }
}

void NetworkCore::disconnectSocket(QTcpSocket *s)
{
    qDebug() << "Disconnecting from" << s->peerAddress();
    s->disconnect(this);
    s->disconnectFromHost();
    connections.removeAll(s);
    if(players.contains(s)) players.remove(s);
    if(s == socket) socket = NULL;
    delete s;
}

void NetworkCore::disconnectAll()
{
    if(getIsServer())
    {
        qDebug() << "Stopping game server...";
        server->disconnect(this);
        server->close();
    }

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

    data << (quint32)0;
    data << packet;
    data.device()->seek(0);
    data << (quint32)(block.size() - sizeof(quint32));

    s->write(block);
}

void NetworkCore::acceptNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    if(!socket) return;

    qDebug() << "New connection from" << socket->peerAddress();
    connections.append(socket);

    setupSocket(socket);
}

void NetworkCore::setupSocket(QTcpSocket *s)
{
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

        if(socket == s) socket = NULL;
        s->close();
        s->deleteLater();

        if(!getIsServer())
        {
            // lost connection to server!
            QMessageBox::critical(0, "Disconnected",
                "Lost connection to game server!");
            GAME->reset();
        }
    }
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

