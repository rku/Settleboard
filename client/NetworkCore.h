/**
 * Settleboard3D
 *
 * This file is part of
 * Settleboard3D - A board game like settlers.
 * Copyright (C) 2010, 2011 Rene Kuettner <rene@bitkanal.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

