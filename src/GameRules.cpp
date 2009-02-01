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

#include "GameRules.h"
#include "Game.h"

// these are the standard rules
// all additional rulesets must inherit from this class

GameRules::GameRules(Game *_game)
    : game(_game)
{
    initActions();
}

void GameRules::initActions()
{
    tradeAct = new QAction(tr("Trade"), this);
    tradeAct->setEnabled(false);
    actions.append(tradeAct);

    buildSettlementAct = new QAction(tr("Build Settlement"), this);
    buildSettlementAct->setEnabled(false);
    actions.append(buildSettlementAct);

    buildCityAct = new QAction(tr("Build City"), this);
    buildCityAct->setEnabled(false);
    actions.append(buildCityAct);

    buildRoadAct = new QAction(tr("Build Road"), this);
    buildRoadAct->setEnabled(false);
    actions.append(buildRoadAct);
}

QList<QAction*> GameRules::getActions()
{
    return actions;
}

const unsigned int GameRules::getWinningPoints()
{
    return winningPoints;
}

void GameRules::setWinningPoints(unsigned int n)
{
    Q_ASSERT(n > 0);
    winningPoints = n;
}

