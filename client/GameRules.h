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

#include <QtGui>
#include <QObject>
#include <QAction>
#include <QList>
#include <QStack>
#include <QLinkedList>
#include <QVariant>
#include <QTcpSocket>

class Game;
class Player;
class GameRules;
class GLGameModel;
class PlayerPanel;
class ControlPanel;
class MessagePanel;
class GameInfoPanel;
class NetworkPacket;

typedef struct _GameRule {
    bool (GameRules::*ruleFunc)(Game*,Player*);
} GameRule;

typedef struct _RuleChainElement {
    Player *player;
    QString name;
    bool suspend;
} RuleChainElement;

// a lot of convenience macros

#define REGISTER_RULE(func) \
    if(1) { \
        GameRule rule; \
        rule.ruleFunc = &GameRules::func; \
        registerRule(#func, rule); \
    }
#define DECLARE_RULE(a) bool a(Game*, Player*);
#define IMPLEMENT_RULE(a) bool GameRules::a(Game *game, Player *player)
#define EXECUTE_SUBRULE(n) executeSubRule(#n, player)
#define EXECUTE_SUBRULE_FOR_PLAYER(n,p) executeSubRule(#n, p)
#define RULECHAIN_ADD(n) \
    if(GAME->getNetworkCore()->getIsServer()) { \
    RuleChainElement _rce; \
    _rce.player = player; _rce.name = #n; _rce.suspend = false; \
    ruleChain.append(_rce); }
#define RULECHAIN_ADD_WITH_PLAYER(n, p) \
    if(GAME->getNetworkCore()->getIsServer()) { \
    RuleChainElement _rce; \
    _rce.player = p; _rce.name = #n; _rce.suspend = false; \
    ruleChain.append(_rce); }
#define RULECHAIN_ADD_TOP(n) \
    if(GAME->getNetworkCore()->getIsServer()) { \
    RuleChainElement _rce; \
    _rce.player = player; _rce.name = #n; _rce.suspend = false; \
    ruleChain.insert(ruleChain.begin(), _rce); }

#define RULEDATA_REQUIRE(n) Q_ASSERT(ruleData.contains(n));
#define RULEDATA_PUSH(n, i) ruleData.insertMulti(n, qVariantFromValue(i))
#define RULEDATA_POP(n)     ruleData.take(n)
#define RULEDATA_READ(n)    ruleData.value(n)

#define LOG_PLAYER_MSG(m) \
    messagePanel->addLogMessage(player, m)
#define LOG_SYSTEM_MSG(m) \
    messagePanel->addSystemMessage(m);

typedef QMap<QString, QVariant> RuleData;
typedef QList<RuleChainElement> RuleChain;

class GameRules : public QObject
{
    Q_OBJECT

    friend class NetworkCore;

    public:
        GameRules(QObject *parent = 0);
        ~GameRules();

        bool executeRule(QString name);
        bool getIsRuleChainWaiting() { return isRuleChainWaiting; }
        void continueRuleChain();
        void cancelRuleChain();
        void pushRuleData(const QString &identifier, QVariant);

    protected:
        void registerRule(QString name, GameRule);
        bool executeRuleFromNetwork(NetworkPacket&);
        bool executeRule(QString name, Player*);
        bool executeSubRule(QString name, Player*);
        void packRuleDataToNetworkPacket(NetworkPacket&);
        void unpackRuleDataFromNetworkPacket(NetworkPacket&);
        void startRuleChain();
        void suspendRuleChain();
        void ruleChainFinished();

        DECLARE_RULE(ruleJoinGame);

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
        DECLARE_RULE(ruleInitDockWidgets);
        DECLARE_RULE(ruleInitPlayerPanel);
        DECLARE_RULE(ruleUpdatePlayerPanel);
        DECLARE_RULE(ruleInitControlPanel);
        DECLARE_RULE(ruleUpdateControlPanel);
        DECLARE_RULE(ruleGenerateBoard);
        DECLARE_RULE(ruleUpdateInterface);

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

        PlayerPanel *playerPanel;
        ControlPanel *controlPanel;
        GameInfoPanel *gameInfoPanel;
        MessagePanel *messagePanel;
        RuleData ruleData;
        RuleChain ruleChain;
        QStack<RuleData> ruleDataStack;
        QStack<RuleChain> ruleChainStack;
        bool isRuleChainWaiting;
        QMap<QString, GameRule> rules;
};

#endif

