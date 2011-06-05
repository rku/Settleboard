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
class ResourceInfoPanel;
class GameCardPanel;
class NetworkPacket;
class GameLobby;

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

#define SERVER_ONLY_RULE \
    if(!GAME->getNetworkCore()->getIsServer()) { \
        qDebug() << "Skipping server-only rule"; return true; }
#define CLIENT_ONLY_RULE \
    if(GAME->getNetworkCore()->getIsServer()) { \
        qDebug() << "Skipping client-only rule"; return true; }
#define LOCAL_ONLY_RULE \
    if(!player->getIsLocal()) { \
        qDebug() << "Skipping local-only rule"; return true; }
#define REMOTE_ONLY_RULE \
    if(player->getIsLocal()) { \
        qDebug() << "Skipping remote-only rule"; return true; }

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
#define RULEDATA_PUSH(n, i) ruleData.insert(n, qVariantFromValue(i))
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

        void reset();
        bool executeRule(QString name);
        bool executeLocalRule(QString name);
        bool getIsRuleChainWaiting() { return isRuleChainWaiting; }
        void continueRuleChain();
        void cancelRuleChain();
        void pushRuleData(QObject*);
        void pushRuleData(const QString &identifier, QVariant);

    protected:
        void registerRule(QString name, GameRule);
        void pushRuleChain();
        void popRuleChain();
        bool executeRuleFromNetwork(NetworkPacket&);
        bool executeRule(QString name, Player*);
        bool executeSubRule(QString name, Player*);
        void packRuleDataToNetworkPacket(NetworkPacket&);
        void unpackRuleDataFromNetworkPacket(NetworkPacket&);
        void startRuleChain();
        void suspendRuleChain();
        void ruleChainFinished();

        DECLARE_RULE(ruleStartServer);
        DECLARE_RULE(ruleJoinGame);
        DECLARE_RULE(rulePlayerJoinedGame);
        DECLARE_RULE(ruleChangePlayerColor);
        DECLARE_RULE(ruleStartPlayerSync);
        DECLARE_RULE(rulePlayerSync);
        DECLARE_RULE(ruleUpdateGameLobby);
        DECLARE_RULE(ruleUpdatePlayerReadyState);
        DECLARE_RULE(ruleNewChatMessage);
        DECLARE_RULE(ruleDisconnect);
        DECLARE_RULE(ruleStartGame);
        DECLARE_RULE(ruleInitGame);
        DECLARE_RULE(ruleInitPlayers);
        DECLARE_RULE(ruleInitGameCards);
        DECLARE_RULE(ruleInitialPlacement);
        DECLARE_RULE(ruleInitialPlacement1);
        DECLARE_RULE(ruleInitialPlacement2);
        DECLARE_RULE(ruleDrawInitialResourceCards);
        DECLARE_RULE(ruleBeginTurn);
        DECLARE_RULE(ruleUserActionEndTurn);
        DECLARE_RULE(ruleEndTurn);
        DECLARE_RULE(ruleUserActionRollDice);
        DECLARE_RULE(ruleDiceRolled);
        DECLARE_RULE(ruleDrawRolledResources);
        DECLARE_RULE(ruleHighlightRolledTiles);
        DECLARE_RULE(ruleDrawCardsFromBankStack);
        DECLARE_RULE(ruleInitDockWidgets);
        DECLARE_RULE(ruleSetCancelable);
        DECLARE_RULE(ruleUnsetCancelable);
        DECLARE_RULE(ruleCancel);
        DECLARE_RULE(ruleCanceled);
        DECLARE_RULE(ruleInitPlayerPanel);
        DECLARE_RULE(ruleUpdatePlayerPanel);
        DECLARE_RULE(ruleInitControlPanel);
        DECLARE_RULE(ruleUpdateControlPanel);
        DECLARE_RULE(ruleUpdateGameInfoPanel);
        DECLARE_RULE(ruleInitResourceInfoPanel);
        DECLARE_RULE(ruleUpdateResourceInfoPanel);
        DECLARE_RULE(ruleUpdateGameCardPanel);
        DECLARE_RULE(ruleGenerateBoard);
        DECLARE_RULE(ruleUpdateInterface);
        DECLARE_RULE(ruleBoardObjectSelected);
        DECLARE_RULE(ruleUserActionMoveRobber);
        DECLARE_RULE(ruleSelectHexTile);
        DECLARE_RULE(ruleMoveRobber);
        DECLARE_RULE(ruleSelectOtherPlayerAtHexTile);
        DECLARE_RULE(ruleStealResourceFromPlayer);
        DECLARE_RULE(ruleDropResources);
        DECLARE_RULE(rulePlayerDropResources);
        DECLARE_RULE(ruleUserActionBuildCity);
        DECLARE_RULE(ruleBuildCity);
        DECLARE_RULE(ruleCanBuildCity);
        DECLARE_RULE(ruleBuyCity);
        DECLARE_RULE(ruleCanBuyCity);
        DECLARE_RULE(ruleSelectSettlement);
        DECLARE_RULE(ruleUserActionBuildSettlement);
        DECLARE_RULE(ruleBuySettlement);
        DECLARE_RULE(ruleCanBuySettlement);
        DECLARE_RULE(ruleBuildSettlement);
        DECLARE_RULE(ruleCanBuildSettlement);
        DECLARE_RULE(ruleRemoveSettlement);
        DECLARE_RULE(ruleSelectCrossroad);
        DECLARE_RULE(ruleCanSelectCrossroad);
        DECLARE_RULE(ruleUserActionBuildRoad);
        DECLARE_RULE(ruleBuildRoad);
        DECLARE_RULE(ruleCanBuildRoad);
        DECLARE_RULE(ruleBuyRoad);
        DECLARE_RULE(ruleCanBuyRoad);
        DECLARE_RULE(ruleSelectRoadway);
        DECLARE_RULE(ruleSelectRoadwayAtCrossroad);
        DECLARE_RULE(ruleCanSelectRoadway);
        DECLARE_RULE(ruleUserActionBuyDevelopmentCard);
        DECLARE_RULE(ruleCanBuyDevelopmentCard);
        DECLARE_RULE(ruleBuyDevelopmentCard);
        DECLARE_RULE(ruleCanPlayKnightCard);
        DECLARE_RULE(ruleCanPlayBuildRoadCard);
        DECLARE_RULE(ruleUserActionPlayBuildRoadCard);
        DECLARE_RULE(rulePlayBuildRoadCard);
        DECLARE_RULE(ruleCanPlayMonopolyCard);
        DECLARE_RULE(ruleCanPlayInventionCard);
        DECLARE_RULE(ruleCanPlayWinningPointCard);

        bool diceRolled;
        unsigned int turn;
        Player *currentPlayer;
        PlayerPanel *playerPanel;
        ControlPanel *controlPanel;
        GameInfoPanel *gameInfoPanel;
        MessagePanel *messagePanel;
        GameCardPanel *gameCardPanel;
        ResourceInfoPanel *resourceInfoPanel;
        RuleData ruleData;
        RuleChain ruleChain;
        QStack<RuleData> ruleDataStack;
        QStack<RuleChain> ruleChainStack;
        bool isRuleChainWaiting;
        QMap<QString, GameRule> rules;
};

#endif

