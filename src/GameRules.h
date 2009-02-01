/**
 * RSETTLE
 *
 * This file is part of
 * RSETTLE - A board game like settlers.
 * Copyright (C) 2007 Rene Kuettner <rene@bitkanal.net>
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

#ifndef GAMERULES_H
#define GAMERULES_H

#include <QObject>
#include <QAction>
#include <QList>

class Game;

class GameRules : public QObject 
{
    Q_OBJECT

    public:
        GameRules(Game*);

        void initActions();

        QList<QAction*> getActions();
        const unsigned int getWinningPoints();
        void setWinningPoints(unsigned int);

    protected:
        Game *game;
        QList<QAction*> actions;
        unsigned int winningPoints;
        QAction *tradeAct;
        QAction *buildSettlementAct;
        QAction *buildCityAct;
        QAction *buildRoadAct;
};

#endif

