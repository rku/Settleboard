
#ifndef BANK_H
#define BANK_H 1

#include <QString>
#include <QMap>

#include "GameCard.h"

class GameCardStack;

class Bank
{
    public:
        Bank();
        ~Bank();

        void reset();

        void registerCardStack(QString stack);
        void registerCard(QString stack, GameCard, uint totalAmount);
        GameCardStack *getCardStack(QString stack);

    protected:
        QMap<QString, GameCardStack*> cardStacks;
};

#endif

