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

#include "GameCardStack.h"

GameCardStack::GameCardStack()
{
}

GameCardStack::~GameCardStack()
{
    // there are cards left on the stack, we release them
    // however, this should not happen during a game
    while(!cards.isEmpty()) delete cards.takeFirst();
}

void GameCardStack::addCard(GameCard *card)
{
    cards.insert(cards.begin(), card);
}

const QList<GameCard*> GameCardStack::getCardsOfType(const QString &type)
{
    QList<GameCard*> foundCards;
    QList<GameCard*>::iterator i;

    for(i = cards.begin(); i != cards.end(); ++i)
    {
        GameCard *c = *i;
        if(c->getType() == type) foundCards.append(c);
    }

    return foundCards;
}

void GameCardStack::shuffle()
{
}

bool GameCardStack::drawFirstCards(GameCardStack *toStack, uint amount)
{
    for(uint i = 0; i < amount; i++)
    {
        if(cards.isEmpty()) return false;
        toStack->addCard(cards.takeFirst());
    }

    return true;
}

bool GameCardStack::drawCardsOfType(GameCardStack *toStack,
    const QString &type, const QString &name, unsigned int amount)
{
    QList<GameCard*>::iterator i;
    QList<GameCard*> foundCards;

    for(i = cards.begin(); i != cards.end(); ++i)
    {
        GameCard *card = *i;
        if(card->getType() == type && card->getName() == name)
            foundCards.append(card);

        if(foundCards.size() == (int)amount) break;
    }

    // have we found enough cards?
    if(foundCards.size() >= (int)amount)
    {
        // draw them
        i = foundCards.begin();
        while(i != foundCards.end())
        {
            cards.removeAll(*i);
            toStack->addCard(*i);
            i++;
        }

        return true;
    }

    return false;
}

uint GameCardStack::getNumberOfCards(QString type)
{
    uint num = 0;

    for(int i = 0; i < cards.size(); i++)
        if(cards.at(i)->getType() == type) num++;

    return num;
}

uint GameCardStack::getNumberOfCards(QString type, QString name)
{
    uint num = 0;

    for(int i = 0; i < cards.size(); i++)
        if(cards.at(i)->getType() == type && cards.at(i)->getName() == name)
            num++;

    return num;
}

