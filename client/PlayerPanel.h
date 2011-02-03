
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

        void registerPlayerInfo(Player*,
                                const QString infoName,
                                const QString description,
                                const QString iconFile = QString(),
                                bool usePlayerColor = false);
        void updatePlayerInfo(Player*, const QString infoName, int value);
        void clear();

    protected:
        QGroupBox *getPlayerBox(Player*);

        QMap<Player*, QString> playerInfos;
        QMap<Player*, QGroupBox*> playerBoxes;
};

#endif

