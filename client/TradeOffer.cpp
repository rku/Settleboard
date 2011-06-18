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

#include "TradeOfferDialog.h"
#include "TradeOffer.h"

TradeOffer::TradeOffer(QObject *parent) : QObject(parent)
{
    clear();
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
    stream << PlayerPtr(obj.object->getFromPlayer());
    stream << PlayerPtr(obj.object->getToPlayer());
    stream << obj.object->getIsBankOnly();
    stream << obj.object->getOfferedResources();
    stream << obj.object->getWantedResources();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, TradeOfferPtr &obj)
{
    QMap<QString, int> offeredResources, wantedResources;
    PlayerPtr fromPlayerPtr, toPlayerPtr;
    bool bankOnly;

    stream >> fromPlayerPtr;
    stream >> toPlayerPtr;
    stream >> bankOnly;
    stream >> offeredResources;
    stream >> wantedResources;

    obj.object = new TradeOffer();
    obj.object->setFromPlayer(fromPlayerPtr.getObject());
    obj.object->setToPlayer(toPlayerPtr.getObject());
    obj.object->setIsBankOnly(bankOnly);
    obj.object->setOfferedResources(offeredResources);
    obj.object->setWantedResources(wantedResources);

    return stream;
}

