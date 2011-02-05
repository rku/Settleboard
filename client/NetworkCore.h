
#ifndef NETWORKCORE_H
#define NETWORKCORE_H 1

#include <Qt>
#include <QTcpSocket>
#include <QTcpServer>

#include "GameObject.h"

class NetworkPacket;

class NetworkCore : public QObject, public GameObject
{
    Q_OBJECT

    public:
        NetworkCore(Game*);
        ~NetworkCore();

        bool startServer(uint port);
        bool getIsServer() { return (server != NULL); }
        bool connectToServer(QString host, uint port);
        void closeConnection(QTcpSocket*);
        void sendPacket(const NetworkPacket&);
        void sendPacket(QTcpSocket*, const NetworkPacket&);

    protected slots:
        void acceptNewConnection();
        void connectionClosed();
        void dataAvailable();
        void connected();

    protected:
        void setupSocket(QTcpSocket*);
        void packetReceived(QTcpSocket*, NetworkPacket&);

        QList<QTcpSocket*> connections;
        QTcpServer *server;
        QTcpSocket *socket;
};

#endif

