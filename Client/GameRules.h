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
#include <QLinkedList>

#include "GameObject.h"

class Game;
class Player;
class GameRules;
class GLGameModel;

typedef struct _GameRule {
    bool (GameRules::*ruleFunc)(_GameRule,Player*);
} GameRule;

typedef struct _RuleChainElement {
    Player *player;
    QString name;
    bool suspend;
} RuleChainElement;

#define REGISTER_RULE(func) \
    if(1) { \
        GameRule rule; \
        rule.ruleFunc = &GameRules::func; \
        registerRule(#func, rule); \
    }
#define DECLARE_RULE(a) bool a(GameRule, Player*);
#define IMPLEMENT_RULE(a) bool GameRules::a(GameRule rule, Player *player)
#define EXECUTE_SUBRULE(n) executeRule(n, player)
#define RULECHAIN_ADD(n) if(1) { \
    RuleChainElement _rce; \
    _rce.player = player; _rce.name = n; _rce.suspend = false; \
    ruleChain.push(_rce); }

class GameRules : public QObject, public GameObject
{
    Q_OBJECT

    public:
        GameRules(Game*);
        ~GameRules();

        void registerRule(QString name, GameRule);
        bool executeRule(QString name, Player*);
        bool getIsRuleChainWaiting() { return isRuleChainWaiting; }
        void continueRuleChain();
        void cancelRuleChain();

        QList<QAction*> getActions();
        unsigned int getWinningPoints();
        void setWinningPoints(unsigned int);

    protected:
        void initActions();
        void startRuleChain();
        void suspendRuleChain();
        void ruleChainFinished();
        
        DECLARE_RULE(ruleInitGameCards);

        DECLARE_RULE(ruleUserActionBuildCity);
        DECLARE_RULE(ruleBuildCity);
        DECLARE_RULE(ruleCanBuildCity);

        DECLARE_RULE(ruleSelectSettlement);
        DECLARE_RULE(ruleSettlementSelected);
        DECLARE_RULE(ruleUserActionBuildSettlement);
        DECLARE_RULE(ruleBuildSettlement);
        DECLARE_RULE(ruleCanBuildSettlement);
        DECLARE_RULE(ruleRemoveSettlement);

        DECLARE_RULE(ruleSelectCrossroad);
        DECLARE_RULE(ruleCrossroadSelected);
        DECLARE_RULE(ruleCanSelectCrossroad);

        DECLARE_RULE(ruleUserActionBuildRoad);
        DECLARE_RULE(ruleBuildRoad);
        DECLARE_RULE(ruleCanBuildRoad);

        DECLARE_RULE(ruleSelectRoadway);
        DECLARE_RULE(ruleRoadwaySelected);
        DECLARE_RULE(ruleCanSelectRoadway);

        QList<QAction*> actions;
        QMap<QString, void*> ruleData;
        QStack<RuleChainElement> ruleChain;
        bool isRuleChainWaiting;
        QMap<QString, GameRule> rules;
        unsigned int winningPoints;
        QAction *tradeAct;
        QAction *buildSettlementAct;
        QAction *buildCityAct;
        QAction *buildRoadAct;
};

#endif

