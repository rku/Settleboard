
#ifndef GAMECARDVIEW_H
#define GAMECARDVIEW_H 1

#include <QLabel>
#include <QMouseEvent>

#include "GameCard.h"

class GameCardView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected);

    public:
        GameCardView(QWidget *parent = 0);

        void clear();

        void setCard(GameCard*);
        GameCard* getCard() { return card; }

        void setIsSelected(bool);
        bool getIsSelected() { return isSelected; }
        void setIsSecret(bool);
        bool getIsSecret() { return isSecret; }

    signals:
        void selected();

    protected:
        void mousePressEvent(QMouseEvent*);

        QLabel *labelPixmap;
        bool isSelected;
        bool isSecret;
        GameCard *card;
};

#endif

