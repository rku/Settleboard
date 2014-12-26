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

#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H 1

#include <QString>
#include <QDataStream>
#include <QMap>
#include <QByteArray>
#include <QtDebug>
#include <QVariant>
#include <QTcpSocket>

#include "Player.h"

#define NETWORK_PACKET_MAGIC    0xCAFE
#define NETWORK_PACKET_VERSION  0x1

class NetworkPacket
{
    public:
        NetworkPacket();
        NetworkPacket(const QString &rule, Player*);
        ~NetworkPacket();

        bool getIsValid();
        void setRuleName(const QString &name) { ruleName = name; }
        const QString &getRuleName() { return ruleName; }
        void addData(const QString &identifier, const QVariant&);
        const QMap<QString, QVariant>& getData() { return data; }
        void setPlayer(Player *p) { player = qVariantFromValue(PlayerPtr(p)); }
        Player *getPlayer() { return player.value<PlayerPtr>().getObject(); }
        void setSocket(QTcpSocket *s) { socket = s; }
        QTcpSocket* getSocket() { return socket; }

        friend QDataStream &operator<<(QDataStream&, const NetworkPacket&);
        friend QDataStream &operator>>(QDataStream&, NetworkPacket&);

    protected:
        quint16 magic;                  // a magic code for packet validation
        uint version;                   // the version of the packet format
        QMap<QString, QVariant> data;   // rule data mapping
        QString ruleName;               // the rule name
        QVariant player;                // the player who sent the packet
        QTcpSocket *socket;             // the socket this packet was received
};

#endif

