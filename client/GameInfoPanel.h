
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

        void setDiceValues(uint die1, uint die2);
        void clearDiceValues();
        void setCurrentPlayer(Player*);
        void setTurn(unsigned int);
        void clear();

    protected:
        QLabel *diceTextLabel;
        QLabel *die1PixmapLabel;
        QLabel *die2PixmapLabel;
        QLabel *currentPlayerLabel;
        QLabel *turnLabel;
};

#endif

