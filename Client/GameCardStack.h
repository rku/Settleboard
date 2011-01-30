
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
        void shuffle();
        bool moveFirstCardsToStack(GameCardStack*, uint amount = 1);
        uint getNumberOfCards() { return cards.size(); }

    protected:
        QList<GameCard*> cards;
};

#endif

