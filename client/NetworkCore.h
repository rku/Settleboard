
#ifndef NETWORKCORE_H
#define NETWORKCORE_H 1

#include <Qt>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDialog>
#include <QTimer>

#include "ui_infoboxform.h"

#define DEFAULT_NETWORK_PORT 1337

class NetworkPacket;
class Player;

class NetworkCore : public QObject
{
    Q_OBJECT

    public:
        NetworkCore(QObject *parent = 0);
        ~NetworkCore();

        bool startServer(uint port = DEFAULT_NETWORK_PORT);
        bool getIsServer() { return server->isListening(); }
        bool getHasConnection() { return (connections.size()>0); }
        bool connectToServer(QString host, uint port);
        void disconnectSocket(QTcpSocket*);
        void sendPacket(const NetworkPacket&);
        void sendPacket(QTcpSocket*, const NetworkPacket&);

        bool getIsClient() { return (!getIsServer()); }
        QTcpSocket* getClientSocket() { return socket; }

    public slots:
        void disconnectAll();

    protected slots:
        void retryConnect();
        void cancelConnect();
        void acceptNewConnection();
        void connectionClosed();
        void dataAvailable();
        void connected();

    protected:
        void setupSocket(QTcpSocket*);
        void packetReceived(QTcpSocket*, NetworkPacket&);

        unsigned int port;
        QString hostName;
        unsigned int connectAttempts;
        QTimer connectTimer;
        Ui::InfoBoxForm infoBox;
        QDialog *infoBoxDialog;
        QList<QTcpSocket*> connections;
        QMap<QTcpSocket*, quint32> pendingBlockSizes;
        QMap<QTcpSocket*, Player*> players;
        QTcpServer *server;
        QTcpSocket *socket;
};

#endif

