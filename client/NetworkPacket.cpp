
#include "NetworkPacket.h"

NetworkPacket::NetworkPacket()
    : magic(NETWORK_PACKET_MAGIC), version(NETWORK_PACKET_VERSION)
{
}

NetworkPacket::NetworkPacket(const QString &rule)
    : magic(NETWORK_PACKET_MAGIC), version(NETWORK_PACKET_VERSION)
{
    setRuleName(rule);
}

NetworkPacket::~NetworkPacket()
{
}

bool NetworkPacket::getIsValid()
{
    return (magic   == NETWORK_PACKET_MAGIC &&
            version == NETWORK_PACKET_VERSION);
}

QDataStream &operator<<(QDataStream &stream, const NetworkPacket &packet)
{
    stream << packet.magic;
    stream << packet.version;
    stream << packet.ruleName;
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
    stream >> packet.data;
    return stream;
}

