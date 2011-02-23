
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

