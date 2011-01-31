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

typedef struct _RuleDataElement {
    uint uintValue;
    int intValue;
    bool boolValue;
    void *pointerValue;
    QString stringValue;
} RuleDataElement;

#define REGISTER_RULE(func) \
    if(1) { \
        GameRule rule; \
        rule.ruleFunc = &GameRules::func; \
        registerRule(#func, rule); \
    }
#define DECLARE_RULE(a) bool a(GameRule, Player*);
#define IMPLEMENT_RULE(a) bool GameRules::a(GameRule rule, Player *player)
#define EXECUTE_SUBRULE(n) executeRule(#n, player)
#define EXECUTE_SUBRULE_FOR_PLAYER(n,p) executeRule(#n, p)
#define RULECHAIN_ADD(n) if(1) { \
    RuleChainElement _rce; \
    _rce.player = player; _rce.name = #n; _rce.suspend = false; \
    ruleChain.append(_rce); }
#define RULECHAIN_ADD_WITH_PLAYER(n, p) if(1) { \
    RuleChainElement _rce; \
    _rce.player = p; _rce.name = #n; _rce.suspend = false; \
    ruleChain.append(_rce); }
#define RULECHAIN_ADD_TOP(n) if(1) { \
    RuleChainElement _rce; \
    _rce.player = player; _rce.name = #n; _rce.suspend = false; \
    ruleChain.insert(ruleChain.begin(), _rce); }

#define RULEDATA_REQUIRE(n) Q_ASSERT(ruleData.contains(n));
#define RULEDATA_PUSH_INT(n, i) \
    if(1) { \
        RuleDataElement _re = { 0, i, false, NULL, QString() }; \
        ruleData.insertMulti(n, _re); }
#define RULEDATA_PUSH_UINT(n, i) \
    if(1) { \
        RuleDataElement _re = { i, 0, false, NULL, QString() }; \
        ruleData.insertMulti(n, _re); }
#define RULEDATA_PUSH_BOOL(n, b) \
    if(1) { \
        RuleDataElement _re = { 0, 0, b, NULL, QString() }; \
        ruleData.insertMulti(n, _re); }
#define RULEDATA_PUSH_POINTER(n, p) \
    if(1) { \
        RuleDataElement _re = { 0, 0, false, p, QString() }; \
        ruleData.insertMulti(n, _re); }
#define RULEDATA_PUSH_STRING(n, s) \
    if(1) { \
        RuleDataElement _re = { 0, 0, false, NULL, s }; \
        ruleData.insertMulti(n, _re); }

#define RULEDATA_POP_INT(n)      ruleData.take(n).intValue
#define RULEDATA_POP_UINT(n)     ruleData.take(n).uintValue
#define RULEDATA_POP_BOOL(n)     ruleData.take(n).boolValue
#define RULEDATA_POP_POINTER(n)  ruleData.take(n).pointerValue
#define RULEDATA_POP_STRING(n)   ruleData.take(n).stringValue
#define RULEDATA_READ_INT(n)     ruleData.value(n).intValue
#define RULEDATA_READ_UINT(n)    ruleData.value(n).uintValue
#define RULEDATA_READ_BOOL(n)    ruleData.value(n).boolValue
#define RULEDATA_READ_POINTER(n) ruleData.value(n).pointerValue
#define RULEDATA_READ_STRING(n)  ruleData.value(n).stringValue


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
        
        DECLARE_RULE(ruleInitGame);
        DECLARE_RULE(ruleInitPlayers);
        DECLARE_RULE(ruleInitGameCards);
        DECLARE_RULE(ruleInitialPlacement);
        DECLARE_RULE(ruleInitialPlacement1);
        DECLARE_RULE(ruleInitialPlacement2);
        DECLARE_RULE(ruleDrawInitialResourceCards);

        DECLARE_RULE(ruleBeginTurn);
        DECLARE_RULE(ruleEndTurn);

        DECLARE_RULE(ruleDrawCardsFromBankStack);

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
        DECLARE_RULE(ruleSelectRoadwayAtCrossroad);
        DECLARE_RULE(ruleRoadwaySelected);
        DECLARE_RULE(ruleCanSelectRoadway);

        QList<QAction*> actions;
        QMap<QString, RuleDataElement> ruleData;
        QList<RuleChainElement> ruleChain;
        bool isRuleChainWaiting;
        QMap<QString, GameRule> rules;
        unsigned int winningPoints;
        QAction *tradeAct;
        QAction *buildSettlementAct;
        QAction *buildCityAct;
        QAction *buildRoadAct;
};

#endif

