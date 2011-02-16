
#ifndef GAMEINFO_PANEL_H
#define GAMEINFO_PANEL_H 1

#include <QtGui>

class Player;

class GameInfoPanel : public QDockWidget
{
    Q_OBJECT

    public:
        GameInfoPanel(const QString &title, QWidget *parent = 0);
        ~GameInfoPanel();

        void setDiceValues(uint dice1, uint dice2);
        void setCurrentPlayer(Player*);
        void setTurn(unsigned int);
        void clear();

    protected:
        QLabel *diceTextLabel;
        QLabel *dice1PixmapLabel;
        QLabel *dice2PixmapLabel;
        QLabel *currentPlayerLabel;
        QLabel *turnLabel;
};

#endif

