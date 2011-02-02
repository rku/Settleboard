
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

uint GameCardStack::getNumberOfCards(QString type)
{
    uint num = 0;

    for(int i = 0; i < cards.size(); i++)
        if(cards.at(0)->type == type) num++;

    return num;
}

uint GameCardStack::getNumberOfCards(QString type, QString name)
{
    uint num = 0;

    for(int i = 0; i < cards.size(); i++)
        if(cards.at(i)->type == type && cards.at(i)->name == name) num++;

    return num;
}

