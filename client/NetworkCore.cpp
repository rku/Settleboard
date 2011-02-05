
#include "NetworkPacket.h"
#include "NetworkCore.h"

NetworkCore::NetworkCore(Game *_game) : GameObject(_game)
{
    server = NULL;
    socket = NULL;
}

NetworkCore::~NetworkCore()
{
    if(socket) delete socket;
    if(server) delete server;
}

bool NetworkCore::startServer(uint port)
{
    if(getIsServer() && server->isListening()) return true;

    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
    qDebug() << "Starting up server to listen at port" << port;

    return server->listen(QHostAddress::Any, port);
}

bool NetworkCore::connectToServer(QString host, uint port)
{
    QTcpSocket *socket = new QTcpSocket(this);
    setupSocket(socket);
    qDebug() << "Connecting to" << host << "on port" << port;
    socket->connectToHost(host, port);
    return true;
}

void NetworkCore::connected()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Connected to" << s->peerAddress();
    connections.append(s);
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
    if(s->state() != QAbstractSocket::ConnectedState)
    {
        closeConnection(s);
        return;
    }

    QDataStream data(s);
    data << packet;
}

void NetworkCore::acceptNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    if(!socket) return;

    qDebug() << "New connection from" << socket->peerAddress();
    connections.append(socket);

    setupSocket(socket);

    NetworkPacket handshakePacket("ruleSendHandshakeRequest");
    sendPacket(socket, handshakePacket);
}

void NetworkCore::setupSocket(QTcpSocket *s)
{
    connect(s, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(s, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(s, SIGNAL(connected()), this, SLOT(connected()));
}

void NetworkCore::connectionClosed()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    if(s)
    {
        qDebug() << "Disconnected from" << s->peerAddress();
        connections.removeAll(s);
    }
}

void NetworkCore::closeConnection(QTcpSocket *s)
{
    qDebug() << "Disconnecting from" << s->peerAddress();
    s->disconnect();
    connections.removeAll(s);
}

void NetworkCore::packetReceived(QTcpSocket *s, NetworkPacket &packet)
{
    qDebug() << "Network Packet received:" << packet.getPacketRule();
}

void NetworkCore::dataAvailable()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    NetworkPacket packet;
    QDataStream data(s);

    data >> packet;
    packetReceived(s, packet);
}

