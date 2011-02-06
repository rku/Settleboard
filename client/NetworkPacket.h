
#ifndef NETWORKPACKET_H
#define NETWORKPACKET_H 1

#include <QString>
#include <QDataStream>
#include <QMap>
#include <QVariant>
#include <QtDebug>

#define NETWORK_PACKET_MAGIC    0xCAFE
#define NETWORK_PACKET_VERSION  0x1

class NetworkPacket
{
    public:
        NetworkPacket();
        NetworkPacket(const QString &rule);
        ~NetworkPacket();

        bool getIsValid();
        void setRuleName(const QString &name) { ruleName = name; }
        const QString &getRuleName() { return ruleName; }
        void addData(const QString &identifier, QVariant &data);

        friend QDataStream &operator<<(QDataStream&, const NetworkPacket&);
        friend QDataStream &operator>>(QDataStream&, NetworkPacket&);

    protected:
        quint16 magic;
        uint version;
        QMap<QString, QVariant> data;
        QString ruleName;
};

#endif

