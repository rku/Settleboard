
#ifndef PLAYER_PANEL_H
#define PLAYER_PANEL_H 1

#include <QtGui>

class Player;

class PlayerPanel : public QWidget
{
    Q_OBJECT

    public:
        PlayerPanel(QWidget *parent);
        ~PlayerPanel();

        void registerPlayerInfo(QString infoName);
        void updatePlayerInfo(Player*, QString infoName, uint value);

        void clear();

    protected:
        void updatePlayerInfos();

        QList<QString> playerInfos;
        QList<QGroupBox*> playerBoxes;
};

#endif

