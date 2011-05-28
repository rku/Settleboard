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

#ifndef GAMECARDSTACK_H
#define GAMECARDSTACK_H 1

#include <QString>
#include <QList>

#include "GameCard.h"

class GameCardStack
{
    public:
        GameCardStack();
        ~GameCardStack();

        void addCard(GameCard*);
        const QList<GameCard*>& getCards() { return cards; }
        const QList<GameCard*> getCardsOfType(const QString &type);
        void shuffle();
        bool drawFirstCards(GameCardStack *toStack, uint amount = 1);
        bool drawCardsOfType(GameCardStack *toStack, const QString &type,
            const QString &name, unsigned int amount = 1);
        uint getNumberOfCards() { return cards.size(); }
        uint getNumberOfCards(QString type);
        uint getNumberOfCards(QString type, QString name);

    protected:
        QList<GameCard*> cards;
};

#endif

