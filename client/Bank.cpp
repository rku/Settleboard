
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

