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

#ifndef TRADEOFFER_H
#define TRADEOFFER_H 1

#include <QtGui>

#include "Player.h"

class TradeOffer : public QObject
{
    Q_OBJECT

    public:
        TradeOffer(QObject *parent = 0);
        TradeOffer(const TradeOffer &other, QObject *parent = 0);

        void show();

        enum TradeOfferState {OfferUnused, OfferAccepted, OfferRejected,
            OfferPlaced};
        void setState(TradeOfferState s) { state = s; }
        TradeOfferState getState() { return state; }

        void setFromPlayer(Player *from);
        Player *getFromPlayer() { return fromPlayer; }
        void setToPlayer(Player *to);
        Player *getToPlayer() { return toPlayer; }

        void setOfferedResource(QString name, int amount);
        void addOfferedResource(QString name, int amount = 1);
        int getOfferedResource(QString name);
        void setWantedResource(QString name, int amount);
        void addWantedResource(QString name, int amount = 1);
        int getWantedResource(QString name);

        void setOfferedResources(QMap<QString, int> &r)
            { offeredResources = r; }
        const QMap<QString, int>& getOfferedResources()
            { return offeredResources; }
        void setWantedResources(QMap<QString, int> &r)
            { wantedResources = r; }
        const QMap<QString, int>& getWantedResources()
            { return wantedResources; }

        bool getIsBankOnly() { return isBankOnly; }
        void setIsBankOnly(bool b) { isBankOnly = b; }

        QUuid getId() { return id; }
        void setId(QUuid theId) { id = theId; }

        void clear();

    private:
        QUuid id;
        TradeOfferState state;
        Player *fromPlayer;
        Player *toPlayer;
        bool isBankOnly;
        QMap<QString, int> wantedResources;
        QMap<QString, int> offeredResources;
};

Q_DECLARE_METATYPE(TradeOffer*);

class TradeOfferPtr
{
    public:
        TradeOfferPtr() : object(NULL) {}
        TradeOfferPtr(TradeOffer *p) { object = p; }
        TradeOfferPtr(const TradeOfferPtr &c) { object = c.object; }
        TradeOffer* getObject() { return object; }

        friend QDataStream &operator<<(QDataStream&, const TradeOfferPtr&);
        friend QDataStream &operator>>(QDataStream&, TradeOfferPtr&);

        TradeOffer *object;
};

Q_DECLARE_METATYPE(TradeOfferPtr);

#endif

