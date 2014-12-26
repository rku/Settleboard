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

#include <QtDebug>

#include "GameCardStack.h"
#include "Bank.h"

Bank::Bank()
{
}

Bank::~Bank()
{
    while(!cardStacks.isEmpty()) delete cardStacks.take(cardStacks.keys()[0]);
}

void Bank::reset()
{
    while(!cardStacks.isEmpty()) delete cardStacks.take(cardStacks.keys()[0]);
}

void Bank::registerCardStack(QString stack)
{
    Q_ASSERT(!cardStacks.contains(stack));
    cardStacks[stack] = new GameCardStack();

    qDebug() << "New card stack" << stack << "created";
}

void Bank::registerCard(QString stack, GameCard card, uint totalAmount)
{
    Q_ASSERT(cardStacks.contains(stack));

    // create totalAmount copies of card
    for(uint i = 0; i < totalAmount; i++)
    {
        GameCard *copiedCard = new GameCard(card);
        cardStacks[stack]->addCard(copiedCard);
    }

    qDebug() << "Created" << totalAmount << card.getName() << "cards for" << stack;
}

GameCardStack* Bank::getCardStack(QString stack)
{
    Q_ASSERT(cardStacks.contains(stack));
    return cardStacks[stack];
}

