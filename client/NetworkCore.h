
#ifndef NETWORKCORE_H
#define NETWORKCORE_H 1

#include <Qt>
#include <QTcpSocket>
#include <QTcpServer>

class NetworkPacket;
class Player;

class NetworkCore : public QObject
{
    Q_OBJECT

    public:
        NetworkCore(QObject *parent = 0);
        ~NetworkCore();

        bool startServer(uint port);
        bool getIsServer() { return server->isListening(); }
        bool getHasConnection() { return (connections.size()>0); }
        bool connectToServer(QString host, uint port);
        void disconnectSocket(QTcpSocket*);
        void disconnectAll();
        void sendPacket(const NetworkPacket&);
        void sendPacket(QTcpSocket*, const NetworkPacket&);

        bool getIsClient() { return (!getIsServer()); }
        QTcpSocket* getClientSocket() { return socket; }

    protected slots:
        void acceptNewConnection();
        void connectionClosed();
        void dataAvailable();
        void connected();

    protected:
        void setupSocket(QTcpSocket*);
        void packetReceived(QTcpSocket*, NetworkPacket&);

        QList<QTcpSocket*> connections;
        QMap<QTcpSocket*, quint32> pendingBlockSizes;
        QMap<QTcpSocket*, Player*> players;
        QTcpServer *server;
        QTcpSocket *socket;
};

#endif

