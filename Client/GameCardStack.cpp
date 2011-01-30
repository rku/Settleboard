
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

bool GameCardStack::moveFirstCardsToStack(GameCardStack *dStack, uint amount)
{
    for(uint i = 0; i < amount; i++)
    {
        if(cards.isEmpty()) return false;
        dStack->addCard(cards.takeFirst());
    }

    return true;
}

