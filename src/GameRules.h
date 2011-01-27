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
#include <QStack>

#include "GameObject.h"

class Game;
class Player;

typedef struct _gameRule {
    void* check_func;
    void* cancel_func;
    void* select_func;
    void* finish_func;
} GameRule;

#define REGISTER_GAMERULE(name, check_f, cancel_f, select_f, finish_f) \
    if(1) { \
        GameRule rule; \
        rule.check_func = check_f; \
        rule.cancel_func = cancel_f; \
        rule.select_func = select_f; \
        rule.finish_func = finish_f; \
        game->getGameRules()->registerRule(name, rule); \
    }

class GameRules : public QObject, public GameObject
{
    Q_OBJECT

    public:
        GameRules(Game*);
        ~GameRules();

        void initActions();

        QList<QAction*> getActions();
        unsigned int getWinningPoints();
        void setWinningPoints(unsigned int);

    protected:
        QList<QAction*> actions;
        QStack<QString> ruleExecutionStack;
        QMap<QString, GameRule> rules;
        unsigned int winningPoints;
        QAction *tradeAct;
        QAction *buildSettlementAct;
        QAction *buildCityAct;
        QAction *buildRoadAct;
};

#endif

