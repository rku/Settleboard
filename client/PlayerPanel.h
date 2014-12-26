/**
 * Settleboard3D
 *
 * This file is part of
 * Settleboard3D - A board game like settlers.
 * Copyright (C) 2010, 2011 Rene Kuettner <rene@bitkanal.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLAYER_PANEL_H
#define PLAYER_PANEL_H 1

#include <QtWidgets>

class Player;

class PlayerPanel : public QDockWidget
{
    Q_OBJECT

    public:
        PlayerPanel(const QString &title, QWidget *parent = 0);
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

        unsigned int columns;
        QMap<Player*, QString> playerInfos;
        QMap<Player*, QGroupBox*> playerBoxes;
};

#endif

