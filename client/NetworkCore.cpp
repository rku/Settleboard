
#include "NetworkCore.h"

NetworkCore::NetworkCore()
{
    server = NULL;
}

NetworkCore::~NetworkCore()
{
    if(server) delete server;
}

bool NetworkCore::startServer(uint port)
{
    if(server && server->isListening()) return true;

    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
    qDebug() << "Starting up server to listen at port" << port;

    return server->listen(QHostAddress::Any, port);
}

bool NetworkCore::connectToServer(QString host, uint port)
{
}

void NetworkCore::sendMessage(const QString msg)
{
    QList<QTcpSocket*>::iterator i;

    for(i = connections.begin(); i != connections.end(); ++i)
    {
        sendMessage((QTcpSocket*)*i, msg);
    }
}

void NetworkCore::closeConnection(QTcpSocket *s)
{
    Q_ASSERT(connections.contains(s));

    qDebug() << "Removing connection to" << s->peerAddress();
    s->disconnect();
    connections.removeAll(s);
}

void NetworkCore::sendMessage(QTcpSocket *s, const QString msg)
{
    if(s->state() != QAbstractSocket::ConnectedState)
    {
        closeConnection(s);
        return;
    }

    QString m = QString("%1\n").arg(msg);
    s->write(qPrintable(m), m.length());
}

void NetworkCore::acceptNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    if(!socket) return;

    qDebug() << "New connection from" << socket->peerAddress();
    connections.append(socket);
    
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));

    sendMessage(socket, "INIT");
}

void NetworkCore::connectionClosed()
{
    QTcpSocket *s = (QTcpSocket*)sender();
    closeConnection(s);
}

