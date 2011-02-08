
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

