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

#include "Game.h"
#include "TradeManager.h"
#include "TradeOfferDialog.h"
#include "TradeOffer.h"

TradeOffer::TradeOffer(QObject *parent) : QObject(parent)
{
    id = QUuid::createUuid();
    GAME->getTradeManager()->addOffer(this); // register

    clear();
}

TradeOffer::TradeOffer(const TradeOffer &other, QObject *parent)
    : QObject(parent)
{
    clear();

    id = other.id;
    state = other.state;
    fromPlayer = other.fromPlayer;
    toPlayer = other.toPlayer;
    isBankOnly = other.isBankOnly;
    offeredResources = other.offeredResources;
    wantedResources = other.wantedResources;
}

TradeOffer::~TradeOffer()
{
}

void TradeOffer::show()
{
    TradeOfferDialog *dlg = new TradeOfferDialog();
    dlg->setTradeOffer(this);
    dlg->exec();
    delete dlg;
}

void TradeOffer::setFromPlayer(Player *from)
{
    fromPlayer = from;
}

void TradeOffer::setToPlayer(Player *to)
{
    toPlayer = to;
}

void TradeOffer::setOfferedResource(QString name, int amount)
{
    offeredResources.insert(name, amount);
}

void TradeOffer::addOfferedResource(QString name, int amount)
{
    int newValue = offeredResources.value(name) + amount;
    offeredResources.insert(name, newValue);
}

int TradeOffer::getOfferedResource(QString name)
{
    Q_ASSERT(offeredResources.contains(name));
    return offeredResources.value(name);
}

QString TradeOffer::getOfferedResourcesAsString()
{
    return resourcesToString(offeredResources);
}

void TradeOffer::setWantedResource(QString name, int amount)
{
    wantedResources.insert(name, amount);
}

void TradeOffer::addWantedResource(QString name, int amount)
{
    int newValue = wantedResources.value(name) + amount;
    wantedResources.insert(name, newValue);
}

int TradeOffer::getWantedResource(QString name)
{
    Q_ASSERT(wantedResources.contains(name));
    return wantedResources.value(name);
}

QString TradeOffer::getWantedResourcesAsString()
{
    return resourcesToString(wantedResources);
}

void TradeOffer::accept()
{
    setState(OfferAccepted);
    sendReply();
}

void TradeOffer::reject()
{
    setState(OfferRejected);
    sendReply();
}

void TradeOffer::execute()
{
    GAME->getRules()->pushRuleData("TradeOfferId", this->getId());
    GAME->getRules()->executeRule("ruleExecuteTrade");
    GAME->getTradeManager()->removeOffer(this);
}

QString TradeOffer::resourcesToString(QMap<QString, int> &resources)
{
    QString str;

    foreach(QString name, resources.keys())
    {
        str.append(QString("%1x %2, ").arg(resources.value(name)).arg(name));
    }

    if(str.length() > 2) str.remove(str.length() - 2, 2);
    return str;
}

void TradeOffer::sendReply()
{
    TradeOffer *reply = new TradeOffer(*this);

    // new id
    reply->newId();

    // swap players
    reply->setToPlayer(getFromPlayer());
    reply->setFromPlayer(getToPlayer());

    // swap resources
    reply->setOfferedResources(getWantedResources());
    reply->setWantedResources(getOfferedResources());

    GAME->getRules()->pushRuleData(reply);
    GAME->getRules()->executeRule("ruleTradeOfferReply");

    delete reply;
}

void TradeOffer::clear()
{
    offeredResources.clear();
    wantedResources.clear();
    isBankOnly = false;
    fromPlayer = NULL;
    toPlayer = NULL;
    state = OfferUnused;
}

// QDataStream operators

QDataStream &operator<<(QDataStream &stream, const TradeOfferPtr &obj)
{
    stream << obj.object->getId();
    stream << (quint8)obj.object->getState();
    stream << obj.object->getFromPlayerId();
    stream << obj.object->getToPlayerId();
    stream << obj.object->getIsBankOnly();
    stream << obj.object->getOfferedResources();
    stream << obj.object->getWantedResources();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, TradeOfferPtr &obj)
{
    QMap<QString, int> offeredResources, wantedResources;
    QString id, fromPlayerId, toPlayerId;
    quint8 state;
    bool bankOnly;

    stream >> id;
    stream >> state;
    stream >> fromPlayerId;
    stream >> toPlayerId;
    stream >> bankOnly;
    stream >> offeredResources;
    stream >> wantedResources;

    obj.object = new TradeOffer();
    obj.object->setId(id);
    obj.object->setState((TradeOffer::TradeOfferState)state);
    obj.object->setFromPlayer(Player::findPlayerWithId(fromPlayerId));
    obj.object->setToPlayer(Player::findPlayerWithId(toPlayerId));
    obj.object->setIsBankOnly(bankOnly);
    obj.object->setOfferedResources(offeredResources);
    obj.object->setWantedResources(wantedResources);

    return stream;
}

