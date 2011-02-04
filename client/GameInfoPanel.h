
#ifndef GAMEINFO_PANEL_H
#define GAMEINFO_PANEL_H 1

#include <QtGui>

class Player;

class GameInfoPanel : public QWidget
{
    Q_OBJECT

    public:
        GameInfoPanel(QWidget *parent);
        ~GameInfoPanel();

        void setDiceValues(uint dice1, uint dice2);
        void setCurrentPlayer(Player*);
        void clear();

    protected:
        QLabel *diceTextLabel;
        QLabel *dice1PixmapLabel;
        QLabel *dice2PixmapLabel;
        QLabel *currentPlayerLabel;
};

#endif

