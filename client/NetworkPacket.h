
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

