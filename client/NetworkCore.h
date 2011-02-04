
#ifndef NETWORKCORE_H
#define NETWORKCORE_H 1

#include <Qt>
#include <QTcpSocket>
#include <QTcpServer>

class NetworkCore : public QObject
{
    Q_OBJECT

    public:
        NetworkCore();
        ~NetworkCore();

        bool startServer(uint port);
        bool connectToServer(QString host, uint port);
        void closeConnection(QTcpSocket*);
        void sendMessage(const QString msg);
        void sendMessage(QTcpSocket*, const QString msg);

    protected slots:
        void acceptNewConnection();
        void connectionClosed();

    protected:
        QList<QTcpSocket*> connections;
        QTcpServer *server;
};

#endif

