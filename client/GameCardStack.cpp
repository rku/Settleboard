
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
        if(card->type == type && card->name == name)
            foundCards.append(card);
    }

    // have we found enough cards?
    if(foundCards.size() >= amount)
    {
        // draw them
        i = foundCards.begin();
        while(i != foundCards.end())
        {
            cards.removeAll(*i);
            toStack->addCard(*i);
        }

        return true;
    }

    return false;
}

uint GameCardStack::getNumberOfCards(QString type)
{
    uint num = 0;

    for(int i = 0; i < cards.size(); i++)
        if(cards.at(i)->type == type) num++;

    return num;
}

uint GameCardStack::getNumberOfCards(QString type, QString name)
{
    uint num = 0;

    for(int i = 0; i < cards.size(); i++)
        if(cards.at(i)->type == type && cards.at(i)->name == name) num++;

    return num;
}

