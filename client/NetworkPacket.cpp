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

#include "NetworkPacket.h"

NetworkPacket::NetworkPacket()
    : magic(NETWORK_PACKET_MAGIC), version(NETWORK_PACKET_VERSION)
{
    socket = NULL;
}

NetworkPacket::NetworkPacket(const QString &rule, Player *_player)
    : magic(NETWORK_PACKET_MAGIC), version(NETWORK_PACKET_VERSION)
{
    setRuleName(rule);
    setPlayer(_player);
    socket = NULL;
}

NetworkPacket::~NetworkPacket()
{
}

bool NetworkPacket::getIsValid()
{
    return (magic   == NETWORK_PACKET_MAGIC &&
            version == NETWORK_PACKET_VERSION);
}

void NetworkPacket::addData(const QString &identifier, const QVariant &d)
{
    data.insert(identifier, d);
}

QDataStream &operator<<(QDataStream &stream, const NetworkPacket &packet)
{
    stream << packet.magic;
    stream << packet.version;
    stream << packet.ruleName;
    stream << packet.player;
    stream << packet.data;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, NetworkPacket &packet)
{
    stream >> packet.magic;
    stream >> packet.version;

    if(packet.magic != NETWORK_PACKET_MAGIC)
    {
        qDebug() << "Invalid packet magic number!";
    }

    if(packet.version != NETWORK_PACKET_VERSION)
    {
        qDebug() << "Invalid packet version number!";
    }

    stream >> packet.ruleName;
    stream >> packet.player;
    stream >> packet.data;
    return stream;
}

