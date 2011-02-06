
#include "Game.h"
#include "Player.h"
#include "NetworkPacket.h"
#include "Crossroad.h"
#include "NetworkCore.h"

NetworkCore::NetworkCore(QObject *parent) : QObject(parent)
{
    server = NULL;
    socket = NULL;
}

NetworkCore::~NetworkCore()
{
    if(socket) delete socket;
    if(server) delete server;
}

Player* NetworkCore::getPlayerForSocket(QTcpSocket *s)
{
    QList<Player*> players = Game::getInstance()->getPlayers();
    QList<Player*>::iterator i;

    for(i = players.begin(); i != players.end(); ++i)
    {
        if((*i)->getSocket() == s) return *i;
    }

    return NULL;
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

    NetworkPacket test("ruleInitGame");
    sendPacket(s,test);
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
    // drop the connection if the hosts sent an invalid packet
    if(!packet.getIsValid())
    {
        qDebug() << "Invalid packet received from" << s->peerAddress();
        s->disconnectFromHost();
        return;
    }

    Player *player = getPlayerForSocket(s);
    qDebug() << "Received rule" << s->peerAddress() << packet.getRuleName();
    Game::getInstance()->getRules()->executeRule(packet.getRuleName(), player);
}

void NetworkCore::dataAvailable()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    NetworkPacket packet;
    QDataStream data(s);

    data >> packet;
    packetReceived(s, packet);
}

