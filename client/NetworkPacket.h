
#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H 1

#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QtDebug>

#define NETWORK_PACKET_MAGIC    0xCAFE
#define NETWORK_PACKET_VERSION  0x1

class NetworkPacket
{
    public:
        NetworkPacket();
        NetworkPacket(const QString &packetRule);
        ~NetworkPacket();

        void setPacketRule(const QString &ruleName) { packetRule = ruleName; }
        const QString &getPacketRule() { return packetRule; }

        friend QDataStream &operator<<(QDataStream&, const NetworkPacket&);
        friend QDataStream &operator>>(QDataStream&, NetworkPacket&);

    protected:
        quint16 magic;
        uint version;
        QByteArray data;
        QString packetRule;
};

#endif

