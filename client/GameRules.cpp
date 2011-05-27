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

#include <time.h>

#include "Crossroad.h"
#include "HexTile.h"
#include "Player.h"
#include "Roadway.h"
#include "PlayerObject.h"
#include "Bank.h"
#include "MainWindow.h"
#include "PlayerPanel.h"
#include "ControlPanel.h"
#include "MessagePanel.h"
#include "GameInfoPanel.h"
#include "ResourceInfoPanel.h"
#include "GameCardPanel.h"
#include "FileManager.h"
#include "NetworkPacket.h"
#include "NetworkCore.h"
#include "GameLobby.h"
#include "Game.h"
#include "StandardMap.h"
#include "GamePixmap.h"
#include "GameCardBrowser.h"
#include "GameRules.h"

GameRules::GameRules(QObject *parent) : QObject(parent)
{
    playerPanel = NULL;
    gameInfoPanel = NULL;
    messagePanel = NULL;
    controlPanel = NULL;
    resourceInfoPanel = NULL;
    gameCardPanel = NULL;

    reset();

    REGISTER_RULE(ruleStartServer);
    REGISTER_RULE(ruleJoinGame);
    REGISTER_RULE(rulePlayerJoinedGame);
    REGISTER_RULE(ruleChangePlayerColor);
    REGISTER_RULE(ruleStartPlayerSync);
    REGISTER_RULE(rulePlayerSync);
    REGISTER_RULE(ruleUpdateGameLobby);
    REGISTER_RULE(ruleUpdatePlayerReadyState);
    REGISTER_RULE(ruleNewChatMessage);
    REGISTER_RULE(ruleDisconnect);
    REGISTER_RULE(ruleStartGame);
    REGISTER_RULE(ruleInitGame);
    REGISTER_RULE(ruleInitPlayers);
    REGISTER_RULE(ruleInitGameCards);
    REGISTER_RULE(ruleInitialPlacement);
    REGISTER_RULE(ruleInitialPlacement1);
    REGISTER_RULE(ruleInitialPlacement2);
    REGISTER_RULE(ruleDrawInitialResourceCards);
    REGISTER_RULE(ruleBeginTurn);
    REGISTER_RULE(ruleEndTurn);
    REGISTER_RULE(ruleUserActionEndTurn);
    REGISTER_RULE(ruleUserActionRollDice);
    REGISTER_RULE(ruleDiceRolled);
    REGISTER_RULE(ruleDrawRolledResources);
    REGISTER_RULE(ruleHighlightRolledTiles);
    REGISTER_RULE(ruleDrawCardsFromBankStack);
    REGISTER_RULE(ruleInitDockWidgets);
    REGISTER_RULE(ruleSetCancelable);
    REGISTER_RULE(ruleUnsetCancelable);
    REGISTER_RULE(ruleCancel);
    REGISTER_RULE(ruleCanceled);
    REGISTER_RULE(ruleInitPlayerPanel);
    REGISTER_RULE(ruleUpdatePlayerPanel);
    REGISTER_RULE(ruleInitControlPanel);
    REGISTER_RULE(ruleUpdateControlPanel);
    REGISTER_RULE(ruleUpdateGameInfoPanel);
    REGISTER_RULE(ruleInitResourceInfoPanel);
    REGISTER_RULE(ruleUpdateResourceInfoPanel);
    REGISTER_RULE(ruleGenerateBoard);
    REGISTER_RULE(ruleUpdateInterface);
    REGISTER_RULE(ruleBoardObjectSelected);
    REGISTER_RULE(ruleUserActionMoveRobber);
    REGISTER_RULE(ruleSelectHexTile);
    REGISTER_RULE(ruleMoveRobber);
    REGISTER_RULE(ruleSelectOtherPlayerAtHexTile);
    REGISTER_RULE(ruleStealResourceFromPlayer);
    REGISTER_RULE(ruleUserActionBuildCity);
    REGISTER_RULE(ruleBuildCity);
    REGISTER_RULE(ruleCanBuildCity);
    REGISTER_RULE(ruleBuyCity);
    REGISTER_RULE(ruleCanBuyCity);
    REGISTER_RULE(ruleSelectSettlement);
    REGISTER_RULE(ruleUserActionBuildSettlement);
    REGISTER_RULE(ruleBuildSettlement);
    REGISTER_RULE(ruleBuySettlement);
    REGISTER_RULE(ruleCanBuySettlement);
    REGISTER_RULE(ruleCanBuildSettlement);
    REGISTER_RULE(ruleRemoveSettlement);
    REGISTER_RULE(ruleSelectCrossroad);
    REGISTER_RULE(ruleCanSelectCrossroad);
    REGISTER_RULE(ruleUserActionBuildRoad);
    REGISTER_RULE(ruleBuildRoad);
    REGISTER_RULE(ruleCanBuildRoad);
    REGISTER_RULE(ruleBuyRoad);
    REGISTER_RULE(ruleCanBuyRoad);
    REGISTER_RULE(ruleSelectRoadway);
    REGISTER_RULE(ruleSelectRoadwayAtCrossroad);
    REGISTER_RULE(ruleCanSelectRoadway);
    REGISTER_RULE(ruleUserActionBuyDevelopmentCard);
    REGISTER_RULE(ruleCanBuyDevelopmentCard);
    REGISTER_RULE(ruleBuyDevelopmentCard);
    REGISTER_RULE(ruleCanPlayKnightCard);
    REGISTER_RULE(ruleCanPlayBuildRoadCard);
    REGISTER_RULE(ruleUserActionPlayBuildRoadCard);
    REGISTER_RULE(rulePlayBuildRoadCard);
    REGISTER_RULE(ruleCanPlayMonopolyCard);
    REGISTER_RULE(ruleCanPlayInventionCard);
    REGISTER_RULE(ruleCanPlayWinningPointCard);
}

GameRules::~GameRules()
{
    reset();
}

void GameRules::reset()
{
    isRuleChainWaiting = false;
    currentPlayer = NULL;
    turn = 0;

    ruleChain.clear();
    ruleData.clear();

    if(playerPanel)
    {
        MainWindow *mainWindow = GAME->getMainWindow();

        mainWindow->removeDockWidget(playerPanel);
        delete playerPanel;
        playerPanel = NULL;

        mainWindow->removeDockWidget(gameInfoPanel);
        delete gameInfoPanel;
        gameInfoPanel = NULL;

        mainWindow->removeDockWidget(controlPanel);
        delete controlPanel;
        controlPanel = NULL;

        mainWindow->removeDockWidget(messagePanel);
        delete messagePanel;
        messagePanel = NULL;

        mainWindow->removeDockWidget(resourceInfoPanel);
        delete resourceInfoPanel;
        resourceInfoPanel = NULL;

        mainWindow->removeDockWidget(gameCardPanel);
        delete gameCardPanel;
        gameCardPanel = NULL;
    }
}

void GameRules::registerRule(QString name, GameRule rule)
{
    // consider registering a rule twice an error
    Q_ASSERT(!rules.contains(name));
    rules.insert(name, rule);

    qDebug() << "Rule registered:" << name;
}

void GameRules::packRuleDataToNetworkPacket(NetworkPacket &packet)
{
    RuleData::iterator i;

    for(i = ruleData.begin(); i != ruleData.end(); ++i)
    {
        QVariant v = i.value();
        const char *typeName = i.value().typeName();

        if(!qstrcmp(typeName, "Crossroad*"))
        { v = qVariantFromValue(CrossroadPtr(i.value().value<Crossroad*>())); }

        else if(!qstrcmp(typeName, "Roadway*"))
        { v = qVariantFromValue(RoadwayPtr(i.value().value<Roadway*>())); }

        else if(!qstrcmp(typeName, "Player*"))
        { v = qVariantFromValue(PlayerPtr(i.value().value<Player*>())); }

        else if(!qstrcmp(typeName, "HexTile*"))
        { v = qVariantFromValue(HexTilePtr(i.value().value<HexTile*>())); }

        qDebug() << "Packing rule data" << i.key() << v;
        packet.addData(i.key(), v);
    }
}

void GameRules::unpackRuleDataFromNetworkPacket(NetworkPacket &packet)
{
    const QMap<QString, QVariant> data = packet.getData();
    QMap<QString, QVariant>::const_iterator i;

    ruleData.clear();

    for(i = data.begin(); i != data.end(); ++i)
    {
        QVariant v = i.value();
        const char *typeName = i.value().typeName();

        if(!qstrcmp(typeName, "CrossroadPtr"))
        { v = qVariantFromValue(i.value().value<CrossroadPtr>().getObject()); }

        else if(!qstrcmp(typeName, "RoadwayPtr"))
        { v = qVariantFromValue(i.value().value<RoadwayPtr>().getObject()); }

        else if(!qstrcmp(typeName, "PlayerPtr"))
        { v = qVariantFromValue(i.value().value<PlayerPtr>().getObject()); }

        else if(!qstrcmp(typeName, "HexTilePtr"))
        { v = qVariantFromValue(i.value().value<HexTilePtr>().getObject()); }

        qDebug() << "Unpacking rule data" << i.key() << v;
        ruleData.insert(i.key(), v);
    }
}

bool GameRules::executeRuleFromNetwork(NetworkPacket &packet)
{
    Player *player = packet.getPlayer();
    Q_ASSERT(player != NULL);

    unpackRuleDataFromNetworkPacket(packet);

    QString name = packet.getRuleName();
    qDebug() << "Executing rule from network:" << name;
    Q_ASSERT(rules.contains(name));

    return executeRule(name, player);
}

bool GameRules::executeRule(QString name)
{
    qDebug() << "Trying to execute rule:" << name;
    Q_ASSERT(rules.contains(name));

    if(!GAME->getNetworkCore()->getHasConnection() ||
        GAME->getNetworkCore()->getIsServer())
    {
        // we are the server, execute it
        executeRule(name, GAME->getLocalPlayer());
    }
    else
    {
        // send the rule to the server
        qDebug() << "Sending rule" << name << "to server";
        NetworkPacket packet(name, GAME->getLocalPlayer());
        packRuleDataToNetworkPacket(packet);
        GAME->getNetworkCore()->sendPacket(packet);
    }

    return true;
}

bool GameRules::executeLocalRule(QString name)
{
    qDebug() << "Executing local rule" << name;
    Q_ASSERT(rules.contains(name));
    Q_ASSERT(currentPlayer != NULL);

    Player *player = GAME->getLocalPlayer();
    GameRule rule = rules.value(name);
    return (this->*rule.ruleFunc)(GAME, player);
}

bool GameRules::executeSubRule(QString name, Player *player)
{
    Q_ASSERT(player != NULL);

    qDebug() << "Executing nested rule" << name << player->getName();
    Q_ASSERT(rules.contains(name));

    GameRule rule = rules.value(name);
    return (this->*rule.ruleFunc)(GAME, player);
}

bool GameRules::executeRule(QString name, Player *player)
{
    Q_ASSERT(player != NULL);
    qDebug() << "Executing rule:" << name << "for" << player->getName();
    Q_ASSERT(rules.contains(name));

    if(GAME->getNetworkCore()->getIsServer())
    {
        // send the rule to the clients
        NetworkPacket packet(name, player);
        packRuleDataToNetworkPacket(packet);
        GAME->getNetworkCore()->sendPacket(packet);
    }

    GameRule rule = rules.value(name);
    return (this->*rule.ruleFunc)(GAME, player);
}

void GameRules::suspendRuleChain()
{
    Q_ASSERT(!isRuleChainWaiting);

    if(ruleChain.size() > 0)
    {
        qDebug() << "Suspending rule chain";
        RuleChainElement rce;
        rce.suspend = true;
        ruleChain.insert(ruleChain.begin(), rce);
    }
}

void GameRules::pushRuleChain()
{
    if(ruleChain.size() > 0)
    {
        qDebug() << "Pushing current rule chain on stack.";
        ruleChainStack.push(ruleChain);
        ruleDataStack.push(ruleData);
        ruleChain.clear();
        ruleData.clear();
    }
}

void GameRules::popRuleChain()
{
    if(ruleChainStack.size() > 0)
    {
        qDebug() << "Popping next rule chain from stack.";
        ruleChain = ruleChainStack.pop();
        ruleData = ruleDataStack.pop();
    }
}

void GameRules::startRuleChain()
{
    if(!GAME->getNetworkCore()->getIsServer()) return;

    Q_ASSERT(ruleChain.size() > 0 && !isRuleChainWaiting);
    qDebug() << "Starting rule chain";

    continueRuleChain();
}

void GameRules::continueRuleChain()
{
    if(!GAME->getNetworkCore()->getIsServer()) return;

    qDebug() << "Continuing rule chain...";

    while(ruleChain.size() > 0)
    {
        RuleChainElement rce = ruleChain.takeFirst();
        isRuleChainWaiting = rce.suspend;
        if(rce.suspend) break;

        // cancel rule, if a rule fails
        if(!executeRule(rce.name, rce.player))
        {
            qDebug() << "Rule" << rce.name << "returned false.";
            cancelRuleChain();
        }

        if(isRuleChainWaiting) break; // detect nested suspensions

        if(ruleChain.size() == 0) popRuleChain();
    }

    if(ruleChain.size() == 0) ruleChainFinished();
    GAME->getBoard()->update();
}

void GameRules::ruleChainFinished()
{
    qDebug() << "Rule chain finished";
    ruleChain.clear();
    ruleData.clear();
    isRuleChainWaiting = false;
    GAME->getBoard()->resetBoardState();
    executeLocalRule("ruleUpdateInterface");
}

void GameRules::cancelRuleChain()
{
    ruleData.clear();
    ruleChain.clear();
    isRuleChainWaiting = false;

    qDebug() << "Rule chain canceled";
}

void GameRules::pushRuleData(QObject *pointer)
{
    Q_ASSERT(pointer != NULL);

#define PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(t) if(1) { \
    t *o = qobject_cast<t*>(pointer); \
    if(o != NULL) { \
        pushRuleData(#t, qVariantFromValue(o)); return; } }

    PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(Crossroad);
    PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(Roadway);
    PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(HexTile);
    PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(Player);

    qDebug() << "Unknown type!";
    Q_ASSERT(false);
}

void GameRules::pushRuleData(const QString &identifier, QVariant data)
{
    ruleData.insert(identifier, data);
}

// STANDARD RULES

IMPLEMENT_RULE(ruleStartServer)
{
    game->getNetworkCore()->startServer();
    game->setState(Game::PreparingState);
    game->getLobby()->show();
    return executeRule("rulePlayerJoinedGame");
}

IMPLEMENT_RULE(ruleJoinGame)
{
    SERVER_ONLY_RULE

    Q_ASSERT(!player->getIsLocal());
    Q_ASSERT(!game->getPlayers().contains(player));

    bool colorInUse = false;
    QList<QColor> availableColors;
    availableColors << Qt::red << Qt::blue << Qt::green << Qt::magenta;

    // check if a player with that name exists
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;
    for(i = players.begin(); i != players.end(); ++i)
    {
        Player *p = *i;

        if(p->getName() == player->getName())
        {
            executeRule("rulePlayerNameExists", player);
            return false;
        }

        if(p->getColor() == player->getColor()) colorInUse = true;
        availableColors.removeAll(p->getColor());
    }

    // check if the player's color is already in use and select
    // a new one if neccessary
    if(colorInUse)
    {
        RULEDATA_PUSH("Color", availableColors.takeFirst());
        executeRule("ruleChangePlayerColor", player);
    }

    // broadcast the join
    executeRule("rulePlayerJoinedGame", player);
    return true;
}

IMPLEMENT_RULE(rulePlayerJoinedGame)
{
    if(!player->getIsLocal())
    {
        Q_ASSERT(!game->getPlayers().contains(player));
        game->getPlayers().append(player);
        game->getLobby()->addChatMessage(
            QString("*** %1 joined the game.").arg(player->getName()),
            Qt::white);
    }
    else
    {
        game->setState(Game::PreparingState);
        game->getLobby()->show();
        game->getLobby()->addChatMessage("*** You joined the game.", Qt::white);
    }

    qDebug() << "Player joined game" << player->getName();
    executeRule("ruleStartPlayerSync");
    return true;
}

IMPLEMENT_RULE(ruleChangePlayerColor)
{
    RULEDATA_REQUIRE("Color");
    QColor color = RULEDATA_POP("Color").value<QColor>();
    player->setColor(color);
    return true;
}

IMPLEMENT_RULE(ruleStartPlayerSync)
{
    SERVER_ONLY_RULE

    QVariantList playerIds;
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;

    for(i = players.begin(); i != players.end(); ++i)
        playerIds.append(qVariantFromValue(PlayerPtr(*i)));

    RULEDATA_PUSH("PlayerIds", playerIds);
    executeRule("rulePlayerSync");

    return EXECUTE_SUBRULE(ruleUpdateGameLobby);
}

IMPLEMENT_RULE(rulePlayerSync)
{
    CLIENT_ONLY_RULE

    RULEDATA_REQUIRE("PlayerIds");
    QVariantList list = RULEDATA_POP("PlayerIds").value<QVariantList>();
    QList<Player*> newList;

    Q_ASSERT(game->getPlayers().count() <= list.count());
    newList.reserve(list.count());

    for(int i = 0; i < list.count(); i++)
    {
        Player *p = list.at(i).value<PlayerPtr>().getObject();
        Q_ASSERT(p);

        if(game->getPlayers().contains(p)) GAME->getPlayers().removeAll(p);
        newList.append(p);
    }

    // all players should habe been arranged
    Q_ASSERT(game->getPlayers().count() == 0);
    game->getPlayers() = newList;

    return EXECUTE_SUBRULE(ruleUpdateGameLobby);
}

IMPLEMENT_RULE(ruleUpdatePlayerReadyState)
{
    RULEDATA_REQUIRE("ReadyState");
    bool isReady = RULEDATA_POP("ReadyState").value<bool>();

    player->setIsReady(isReady);
    game->getLobby()->addChatMessage(QString("*** %1 is now %2.")
        .arg(player->getName()).arg((isReady)?"ready":"not ready"), 
        Qt::white);
    game->getLobby()->update();

    return true;
}

IMPLEMENT_RULE(ruleNewChatMessage)
{
    RULEDATA_REQUIRE("ChatMessage");
    QString message = RULEDATA_POP("ChatMessage").value<QString>();

    // add message to game lobby
    QString finalMsg = QString("<%1> %2").arg(player->getName()).arg(message);
    game->getLobby()->addChatMessage(finalMsg, player->getColor());

    if(game->getState() == Game::PlayingState)
        messagePanel->addChatMessage(player, message);

    return true;
}

IMPLEMENT_RULE(ruleDisconnect)
{
    // this should never happen!
    Q_ASSERT(!player->getIsLocal());

    player->setIsDisconnected(true);

    if(game->getState() == Game::PreparingState)
    {
        game->getPlayers().removeAll(player);
        game->getLobby()->addChatMessage(
            QString("*** %1 left the game.").arg(player->getName()),
            Qt::white);
        game->getLobby()->update();
        delete player;
    }
    else EXECUTE_SUBRULE(ruleUpdateInterface);

    return true;
}

IMPLEMENT_RULE(ruleUpdateGameLobby)
{
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;

    game->getLobby()->update();
    return true;
}

IMPLEMENT_RULE(ruleStartGame)
{
    qsrand(time(NULL));
    diceRolled = false;
    game->getLobby()->hide();
    game->setState(Game::PlayingState);
    EXECUTE_SUBRULE(ruleInitGame);
    return true;
}

IMPLEMENT_RULE(ruleInitGame)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleInitGameCards);
    RULECHAIN_ADD(ruleInitPlayers);
    RULECHAIN_ADD(ruleInitDockWidgets);
    RULECHAIN_ADD(ruleInitPlayerPanel);
    RULECHAIN_ADD(ruleInitControlPanel);
    RULECHAIN_ADD(ruleInitResourceInfoPanel);
    RULECHAIN_ADD(ruleGenerateBoard);
    RULECHAIN_ADD(ruleInitialPlacement);
    RULECHAIN_ADD(ruleBeginTurn)
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleInitialPlacement1)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleSelectCrossroad);
    RULECHAIN_ADD(ruleBuildSettlement);
    RULECHAIN_ADD(ruleSelectRoadwayAtCrossroad);
    RULECHAIN_ADD(ruleBuildRoad);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleInitialPlacement2)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleSelectCrossroad);
    RULECHAIN_ADD(ruleDrawInitialResourceCards);
    RULECHAIN_ADD(ruleBuildSettlement);
    RULECHAIN_ADD(ruleSelectRoadwayAtCrossroad);
    RULECHAIN_ADD(ruleBuildRoad);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleInitialPlacement)
{
    SERVER_ONLY_RULE

    QList<Player*> players = game->getPlayers();
    QListIterator<Player*> i(players);

    pushRuleChain();

    while(i.hasNext())
        RULECHAIN_ADD_WITH_PLAYER(ruleInitialPlacement1, i.next());

    i.toBack();

    while(i.hasPrevious())
        RULECHAIN_ADD_WITH_PLAYER(ruleInitialPlacement2, i.previous());

    startRuleChain();
    return true;
}

IMPLEMENT_RULE(ruleBeginTurn)
{
    diceRolled = false;
    turn++;
    gameInfoPanel->setTurn(turn);

    currentPlayer = player;
    LOG_PLAYER_MSG(QString("%1 begins turn %2.")
        .arg(player->getName()).arg(turn));

    EXECUTE_SUBRULE(ruleUpdateInterface);
    return true;
}

IMPLEMENT_RULE(ruleUserActionEndTurn)
{
    SERVER_ONLY_RULE

    executeRule("ruleEndTurn", player);
    return true;
}

IMPLEMENT_RULE(ruleEndTurn)
{
    QList<Player*> players = game->getPlayers();
    Q_ASSERT(players.contains(player));

    LOG_PLAYER_MSG(QString("%1 finished turn %2.")
        .arg(player->getName()).arg(turn));

    gameInfoPanel->clearDiceValues();

    // hand over to next player
    unsigned int index = players.indexOf(player) + 1;
    if(players.size() <= (int)index) index = 0;

    return executeSubRule("ruleBeginTurn", players.at(index));
}

IMPLEMENT_RULE(ruleUserActionRollDice)
{
    SERVER_ONLY_RULE

    pushRuleChain();

    quint8 die1 = quint8( qrand() / (RAND_MAX + 1.0) * 6 + 1 );
    quint8 die2 = quint8( qrand() / (RAND_MAX + 1.0) * 6 + 1 );

    RULEDATA_PUSH("Die1Value", die1);
    RULEDATA_PUSH("Die2Value", die2);

    RULECHAIN_ADD(ruleDiceRolled);

    if((die1+die2) == 7)
    {
        RULECHAIN_ADD(ruleSelectHexTile);
        RULECHAIN_ADD(ruleMoveRobber);
        RULECHAIN_ADD(ruleSelectOtherPlayerAtHexTile);
        RULECHAIN_ADD(ruleStealResourceFromPlayer);
    }
    else
    {
        RULECHAIN_ADD(ruleDrawRolledResources);
        RULECHAIN_ADD(ruleHighlightRolledTiles);
    }

    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleDiceRolled)
{
    RULEDATA_REQUIRE("Die1Value");
    RULEDATA_REQUIRE("Die2Value");
    quint8 die1 = RULEDATA_READ("Die1Value").value<quint8>();
    quint8 die2 = RULEDATA_READ("Die2Value").value<quint8>();

    diceRolled = true;
    LOG_PLAYER_MSG(QString("%1 rolled a %2 (%3, %4).")
        .arg(player->getName()).arg(die1 + die2).arg(die1).arg(die2));

    gameInfoPanel->setDiceValues(die1, die2);

    return true;
}

IMPLEMENT_RULE(ruleDrawRolledResources)
{
    SERVER_ONLY_RULE

    RULEDATA_REQUIRE("Die1Value");
    RULEDATA_REQUIRE("Die2Value");
    quint8 die1 = RULEDATA_READ("Die1Value").value<quint8>();
    quint8 die2 = RULEDATA_READ("Die2Value").value<quint8>();

    HexTileList tiles = game->getBoard()->getBoardTiles();
    HexTileList::iterator hI;

    for(hI = tiles.begin(); hI != tiles.end(); ++hI)
    {
        HexTile *t = *hI;
        QString stack;

        if(!t->getHasNumberChip()) continue;
        if(t->getHasRobber()) continue;
        if(t->getChipNumber() != (die1+die2)) continue;

        stack = t->getResourceName();
        if(stack.isEmpty()) continue;

        QList<Crossroad*> crossroads = t->getCrossroads();
        QList<Crossroad*>::iterator cI;
        for(cI = crossroads.begin(); cI != crossroads.end(); ++cI)
        {
            unsigned int amount = 0;
            Crossroad *c = *cI;

            if(!c->getIsPlayerObjectPlaced()) continue;
            PlayerObject *pO = c->getPlayerObject();

            if(pO->getType() == "Settlement") amount = 1;
            if(pO->getType() == "City") amount = 2;

            if(amount > 0)
            {
                ruleData.remove("CardStackName");
                ruleData.remove("Amount");
                RULEDATA_PUSH("CardStackName", stack);
                RULEDATA_PUSH("Amount", amount);
                executeRule("ruleDrawCardsFromBankStack", pO->getOwner());
            }
        }
    }

    return true;
}

IMPLEMENT_RULE(ruleHighlightRolledTiles)
{
    RULEDATA_REQUIRE("Die1Value");
    RULEDATA_REQUIRE("Die2Value");
    quint8 die1 = RULEDATA_READ("Die1Value").value<quint8>();
    quint8 die2 = RULEDATA_READ("Die2Value").value<quint8>();

    // IMPLEMENT ME!
    return true;
}

IMPLEMENT_RULE(ruleDrawInitialResourceCards)
{
    RULEDATA_REQUIRE("Crossroad");

    Crossroad *r = RULEDATA_READ("Crossroad").value<Crossroad*>();
    RULEDATA_PUSH("Amount", 1);

    for(int i = 0; i < r->getTiles().size(); i++)
    {
        QString stack = r->getTiles().at(i)->getResourceName();
        if(stack.isEmpty()) continue;

        RULEDATA_PUSH("CardStackName", stack);
        EXECUTE_SUBRULE(ruleDrawCardsFromBankStack);
    }

    return true;
}

// Used ruleData objects:
//      Amount -> uint
//      CardStackname -> QString
IMPLEMENT_RULE(ruleDrawCardsFromBankStack)
{
    RULEDATA_REQUIRE("Amount");
    RULEDATA_REQUIRE("CardStackName");

    Bank *bank = game->getBank();
    QString stackName = RULEDATA_POP("CardStackName").value<QString>();
    uint amount = RULEDATA_READ("Amount").value<uint>();
    GameCardStack *stack = bank->getCardStack(stackName);

    qDebug() << "Player" << player->getName() << "draws" << amount
        << "cards from bank stack" << stackName;

    bool r = stack->drawFirstCards(player->getCardStack(), amount);
    EXECUTE_SUBRULE(ruleUpdateInterface);

    LOG_PLAYER_MSG(QString("%1 draws %2 %3 card from the bank.")
        .arg(player->getName()).arg(amount).arg(stackName));

    return r;
}

IMPLEMENT_RULE(ruleInitPlayers)
{
    QList<Player*> players = game->getPlayers();

    // initialize player objects for each player
    for(int i = 0; i < players.size(); i++)
    {
        Player *p = players.at(i);

        for(int i = 0; i < 15; i++) p->addObjectOfType("Road");
        for(int i = 0; i < 5;  i++) p->addObjectOfType("Settlement");
        for(int i = 0; i < 4;  i++) p->addObjectOfType("City");
        p->setWinningPoints(0);

        qDebug() << "Player" << p->getName() << "initialized";
    }

    return true;
}

IMPLEMENT_RULE(ruleInitGameCards)
{
    Bank *bank = game->getBank();

    bank->registerCardStack("Discarded"); // the stack for discarded cards

    // resources
    bank->registerCardStack("Wheat");
    bank->registerCardStack("Wool");
    bank->registerCardStack("Ore");
    bank->registerCardStack("Lumber");
    bank->registerCardStack("Clay");

    bank->registerCard("Wheat",  GAMECARD_WHEAT,  19);
    bank->registerCard("Wool",   GAMECARD_SHEEP,  19);
    bank->registerCard("Ore",    GAMECARD_ORE,    19);
    bank->registerCard("Lumber", GAMECARD_LUMBER, 19);
    bank->registerCard("Clay",   GAMECARD_CLAY,   19);

    // development cards
    bank->registerCardStack("Development");
    bank->registerCard("Development", GAMECARD_KNIGHT, 14);
    bank->registerCard("Development", GAMECARD_BUILD_ROAD, 2);
    bank->getCardStack("Development")->shuffle();

    return true;
}

IMPLEMENT_RULE(ruleInitDockWidgets)
{
    QMainWindow *mainWindow = game->getMainWindow();

    playerPanel = new PlayerPanel("", mainWindow);
    mainWindow->addDockWidget(Qt::BottomDockWidgetArea, playerPanel);

    gameInfoPanel = new GameInfoPanel("", mainWindow);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, gameInfoPanel);

    controlPanel = new ControlPanel("", mainWindow);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, controlPanel);

    resourceInfoPanel = new ResourceInfoPanel("", mainWindow);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, resourceInfoPanel);

    gameCardPanel = new GameCardPanel("", mainWindow);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, gameCardPanel);

    messagePanel = new MessagePanel("", mainWindow);
    mainWindow->addDockWidget(Qt::BottomDockWidgetArea, messagePanel);

    return true;
}

IMPLEMENT_RULE(ruleSetCancelable)
{
    LOCAL_ONLY_RULE

    controlPanel->beginCancelMode();
    return true;
}

IMPLEMENT_RULE(ruleUnsetCancelable)
{
    LOCAL_ONLY_RULE

    controlPanel->endCancelMode();
    return true;
}

IMPLEMENT_RULE(ruleCancel)
{
    SERVER_ONLY_RULE

    cancelRuleChain();
    return executeRule("ruleCanceled", player);
}

IMPLEMENT_RULE(ruleCanceled)
{
    Board *board = game->getBoard();
    if(board->getIsSelectionModeActive()) board->endSelectionMode();

    LOG_SYSTEM_MSG(QString("%1 canceled the current action.")
        .arg(player->getName()));

    EXECUTE_SUBRULE(ruleUpdateInterface);

    return true;
}

IMPLEMENT_RULE(ruleInitPlayerPanel)
{
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;

    Q_ASSERT(players.size() > 0);

    for(i = players.begin(); i != players.end(); ++i)
    {
        playerPanel->registerPlayerInfo(*i,
            "WinningPoints", "Winning Points", "WinningPoints.png");
        playerPanel->registerPlayerInfo(*i,
            "Roads", "Available Roads", "Road.png", true);
        playerPanel->registerPlayerInfo(*i,
            "Settlements", "Available Settlements", "Settlement.png", true);
        playerPanel->registerPlayerInfo(*i,
            "Cities", "Available Cities", "City.png", true);
        playerPanel->registerPlayerInfo(*i,
            "DevelopmentCards", "Development Cards", "DevCard.png", true);
        playerPanel->registerPlayerInfo(*i,
            "ResourceCards", "Resource Cards", "Card.png", true);
        
        EXECUTE_SUBRULE_FOR_PLAYER(ruleUpdatePlayerPanel, *i);
    }

    return true;
}

IMPLEMENT_RULE(ruleUpdatePlayerPanel)
{
    playerPanel->updatePlayerInfo(player, "WinningPoints",
        player->getWinningPoints());
    playerPanel->updatePlayerInfo(player, "Roads",
        player->getNumberOfUnplacedObjectsOfType("Road"));
    playerPanel->updatePlayerInfo(player, "Settlements",
        player->getNumberOfUnplacedObjectsOfType("Settlement"));
    playerPanel->updatePlayerInfo(player, "Cities",
        player->getNumberOfUnplacedObjectsOfType("City"));
    playerPanel->updatePlayerInfo(player, "DevelopmentCards",
        player->getCardStack()->getNumberOfCards("Development"));
    playerPanel->updatePlayerInfo(player, "ResourceCards",
        player->getCardStack()->getNumberOfCards("Resource"));

    return true;
}

IMPLEMENT_RULE(ruleInitControlPanel)
{
    QColor color = game->getLocalPlayer()->getColor();

    QAction *actionBuildRoad = new QAction(controlPanel);
    actionBuildRoad->setData(QString("ruleUserActionBuildRoad"));
    actionBuildRoad->setToolTip("Build Road");
    actionBuildRoad->setIcon(GamePixmap("Road.png", color).toIcon());
    controlPanel->registerAction("BuildRoad", actionBuildRoad);

    QAction *actionBuildSettlement = new QAction(controlPanel);
    actionBuildSettlement->setData(QString("ruleUserActionBuildSettlement"));
    actionBuildSettlement->setToolTip("Build Settlement");
    actionBuildSettlement->setIcon(GamePixmap("Settlement.png", color).toIcon());
    controlPanel->registerAction("BuildSettlement", actionBuildSettlement);

    QAction *actionBuildCity = new QAction(controlPanel);
    actionBuildCity->setData(QString("ruleUserActionBuildCity"));
    actionBuildCity->setToolTip("Build City");
    actionBuildCity->setIcon(GamePixmap("City.png", color).toIcon());
    controlPanel->registerAction("BuildCity", actionBuildCity);

    QAction *actionBuyDevCard = new QAction(controlPanel);
    actionBuyDevCard->setData(QString("ruleUserActionBuyDevelopmentCard"));
    actionBuyDevCard->setToolTip("Buy Development Card");
    actionBuyDevCard->setIcon(GamePixmap("DevCard.png", color).toIcon());
    controlPanel->registerAction("BuyDevCard", actionBuyDevCard);

    QAction *actionTrade = new QAction(controlPanel);
    actionTrade->setData(QString("ruleUserActionTrade"));
    actionTrade->setToolTip("Trade");
    actionTrade->setIcon(GamePixmap("Trade.png", color).toIcon());
    controlPanel->registerAction("Trade", actionTrade);

    QAction *actionShowCards = new QAction(controlPanel);
    actionShowCards->setData(QString());
    actionShowCards->setToolTip("Show my cards");
    actionShowCards->setIcon(GamePixmap("ShowCards.png", color).toIcon());
    connect(actionShowCards, SIGNAL(triggered()),
        game, SLOT(browseLocalGameCards()));
    controlPanel->registerAction("ShowCards", actionShowCards);

    QAction *actionRollDice = new QAction(controlPanel);
    actionRollDice->setData(QString("ruleUserActionRollDice"));
    actionRollDice->setToolTip("Roll Dice");
    actionRollDice->setIcon(GamePixmap("RollDice.png", color).toIcon());
    controlPanel->registerAction("RollDice", actionRollDice);

    QAction *actionEndTurn = new QAction(controlPanel);
    actionEndTurn->setData(QString("ruleUserActionEndTurn"));
    actionEndTurn->setToolTip("End Turn");
    actionEndTurn->setIcon(GamePixmap("EndTurn.png", color).toIcon());
    controlPanel->registerAction("EndTurn", actionEndTurn);

    EXECUTE_SUBRULE(ruleUpdateControlPanel);
    return true;
}

IMPLEMENT_RULE(ruleUpdateControlPanel)
{
    controlPanel->setActionState("BuildRoad", false);
    controlPanel->setActionState("BuildSettlement", false);
    controlPanel->setActionState("BuildCity", false);
    controlPanel->setActionState("BuyDevCard", false);
    controlPanel->setActionState("ShowCards", false);
    controlPanel->setActionState("Trade", false);
    controlPanel->setActionState("RollDice", false);
    controlPanel->setActionState("EndTurn", false);

    if(currentPlayer && currentPlayer->getIsLocal())
    {
        controlPanel->setActionState("BuildRoad",
            diceRolled &&
            EXECUTE_SUBRULE(ruleCanBuildRoad) &&
            EXECUTE_SUBRULE(ruleCanBuyRoad));
        controlPanel->setActionState("BuildSettlement",
            diceRolled &&
            EXECUTE_SUBRULE(ruleCanBuildSettlement) &&
            EXECUTE_SUBRULE(ruleCanBuySettlement));
        controlPanel->setActionState("BuildCity",
            diceRolled &&
            EXECUTE_SUBRULE(ruleCanBuildCity) &&
            EXECUTE_SUBRULE(ruleCanBuyCity));
        controlPanel->setActionState("ShowCards",
            (player->getCardStack()->getNumberOfCards("Development") > 0));
        controlPanel->setActionState("BuyDevCard",
            diceRolled &&
            EXECUTE_SUBRULE(ruleCanBuyDevelopmentCard));
        controlPanel->setActionState("RollDice", !diceRolled);
        controlPanel->setActionState("EndTurn", diceRolled);
    }

    return true;
}

IMPLEMENT_RULE(ruleUpdateGameInfoPanel)
{
    if(currentPlayer) gameInfoPanel->setCurrentPlayer(currentPlayer);
    return true;
}

IMPLEMENT_RULE(ruleInitResourceInfoPanel)
{
    resourceInfoPanel->registerResource("Lumber");
    resourceInfoPanel->registerResource("Clay");
    resourceInfoPanel->registerResource("Wool");
    resourceInfoPanel->registerResource("Wheat");
    resourceInfoPanel->registerResource("Ore");

    return true;
}

IMPLEMENT_RULE(ruleUpdateResourceInfoPanel)
{
    Player *p = game->getLocalPlayer();
    GameCardStack *stack = p->getCardStack();

    resourceInfoPanel->updateResource("Wheat",
        stack->getNumberOfCards("Resource", "Wheat"));
    resourceInfoPanel->updateResource("Lumber",
        stack->getNumberOfCards("Resource", "Lumber"));
    resourceInfoPanel->updateResource("Wool",
        stack->getNumberOfCards("Resource", "Wool"));
    resourceInfoPanel->updateResource("Ore",
        stack->getNumberOfCards("Resource", "Ore"));
    resourceInfoPanel->updateResource("Clay",
        stack->getNumberOfCards("Resource", "Clay"));

    return true;
}

IMPLEMENT_RULE(ruleUpdateInterface)
{
    EXECUTE_SUBRULE(ruleUpdatePlayerPanel);
    EXECUTE_SUBRULE(ruleUpdateControlPanel);
    EXECUTE_SUBRULE(ruleUpdateGameInfoPanel);
    EXECUTE_SUBRULE(ruleUpdateResourceInfoPanel);

    game->getBoard()->update();
    return true;
}

IMPLEMENT_RULE(ruleBoardObjectSelected)
{
    SERVER_ONLY_RULE

    if(getIsRuleChainWaiting()) continueRuleChain();
    return true;
}

IMPLEMENT_RULE(ruleGenerateBoard)
{
    game->getBoard()->load(new StandardMap(game->getBoard()));
    game->getBoard()->update();
    return true;
}

IMPLEMENT_RULE(ruleUserActionMoveRobber)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleSelectHexTile);
    RULECHAIN_ADD(ruleMoveRobber);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleSelectHexTile)
{
    bool selectableObjectFound = false;
    Board *board = game->getBoard();
    QList<HexTile*> tiles = board->getBoardTiles();

    for(int i = 0; i < tiles.size(); i++)
    {
        HexTile *h = tiles.at(i);
        if(h->getType() != HexTile::WaterType && !h->getHasRobber())
        {
            if(player->getIsLocal()) h->setIsSelectable(true);
            selectableObjectFound = true;
        }
    }

    if(selectableObjectFound)
    {
        if(player->getIsLocal()) board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("Waiting for %1 to select a board tile.").
            arg(player->getName()))
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleMoveRobber)
{
    RULEDATA_REQUIRE("HexTile");
    HexTile *tile = RULEDATA_READ("HexTile").value<HexTile*>();
    Q_ASSERT(tile != NULL);

    // clear current robber
    QList<HexTile*> tiles = game->getBoard()->getBoardTiles();
    QList<HexTile*>::iterator i;
    for(i = tiles.begin(); i != tiles.end(); ++i)
        if((*i)->getHasRobber()) (*i)->setHasRobber(false);

    // set new robber
    tile->setHasRobber(true);

    return true;
}

IMPLEMENT_RULE(ruleSelectOtherPlayerAtHexTile)
{
    RULEDATA_REQUIRE("HexTile");
    HexTile *tile = RULEDATA_READ("HexTile").value<HexTile*>();
    Q_ASSERT(tile != NULL);

    qDebug() << "Getting players at tile...";

    // collect player buildings
    int selectableFound = 0;
    Player *selectedPlayer = NULL;
    Board *board = game->getBoard();
    QList<Crossroad*> crossroads = board->getCrossroads();
    QList<Crossroad*>::iterator iC;
    for(iC = crossroads.begin(); iC != crossroads.end(); ++iC)
    {
        Crossroad *c = *iC;
        if(!c->getIsPlayerObjectPlaced()) continue;

        PlayerObject *obj = c->getPlayerObject();
        if(obj->getOwner() == player) continue;

        obj->setIsSelectable(true);
        if(selectedPlayer != obj->getOwner()) selectableFound++;
        selectedPlayer = obj->getOwner();
        qDebug() << "Found player at tile:" << selectedPlayer->getName();
    }

    if(selectableFound > 0)
    {
        // only one hit - select it
        if(selectableFound == 1)
        {
            RULEDATA_PUSH("Player", selectedPlayer);
            qDebug() << "Automatically selected player:" <<
                selectedPlayer->getName();
            return true;
        }

        // more than one hit - let the user decide
        if(player->getIsLocal()) board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("Waiting for %1 to select a player.").
            arg(player->getName()))
        return true;
    }

    LOG_SYSTEM_MSG("No player found to steal from.");
    return true;
}

IMPLEMENT_RULE(ruleStealResourceFromPlayer)
{
    if(!ruleData.contains("Player")) return false;

    Player *p = RULEDATA_READ("Player").value<Player*>();
    Q_ASSERT(p != NULL);

    qDebug() << player->getName() << "is about to steal a resource from"
        << p->getName();

    return true;
}

IMPLEMENT_RULE(ruleUserActionBuildCity)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleCanBuildCity);
    RULECHAIN_ADD(ruleCanBuyCity);
    RULECHAIN_ADD(ruleSetCancelable);
    RULECHAIN_ADD(ruleSelectSettlement);
    RULECHAIN_ADD(ruleUnsetCancelable);
    RULECHAIN_ADD(ruleBuyCity);
    RULECHAIN_ADD(ruleBuildCity);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleBuildCity)
{
    RULEDATA_REQUIRE("Crossroad");
    Crossroad *cr = RULEDATA_POP("Crossroad").value<Crossroad*>();

    // remove settlement
    Q_ASSERT(cr->getIsPlayerObjectPlaced());
    RULEDATA_PUSH("Settlement", cr->getPlayerObject());
    if(EXECUTE_SUBRULE(ruleRemoveSettlement))
    {
        // build city
        PlayerObject *bld = player->getUnplacedObjectOfType("City");
        bld->setScale(0.7);
        cr->placePlayerObject(bld);
        player->increaseWinningPointsBy(2);

        EXECUTE_SUBRULE(ruleUpdateInterface);
        LOG_PLAYER_MSG(QString("%1 built a City.").arg(player->getName()));
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleCanBuildCity)
{
    unsigned int nC = player->getNumberOfUnplacedObjectsOfType("City");
    unsigned int nS = player->getNumberOfPlacedObjectsOfType("Settlement");

    if(nC < 1 || nS < 1) return false;

    return true;
}

IMPLEMENT_RULE(ruleBuyCity)
{
    GameCardStack *oreStack = game->getBank()->getCardStack("Ore");
    GameCardStack *wheatStack = game->getBank()->getCardStack("Wheat");
    GameCardStack *playerStack = player->getCardStack();

    playerStack->drawCardsOfType(oreStack, "Resource", "Ore", 2);
    playerStack->drawCardsOfType(wheatStack, "Resource", "Wheat", 3);

    LOG_PLAYER_MSG(QString("%1 payed 2 ore and 3 wheat.")
        .arg(player->getName()));

    return true;
}

IMPLEMENT_RULE(ruleCanBuyCity)
{
    GameCardStack *stack = player->getCardStack();
    unsigned int nOre = stack->getNumberOfCards("Resource", "Ore");
    unsigned int nWheat = stack->getNumberOfCards("Resource", "Wheat");

    if(nOre < 2 || nWheat < 3) return false;

    return true;
}

IMPLEMENT_RULE(ruleSelectSettlement)
{
    bool selectableObjectFound = false;
    Board *board = game->getBoard();
    QList<Crossroad*> crossroads = board->getCrossroads();

    for(int i = 0; i < crossroads.size(); i++)
    {
        Crossroad *c = crossroads.at(i);
        if(c->getIsPlayerObjectPlaced())
        {
            PlayerObject *bld = (PlayerObject*)c->getPlayerObject();
            if(!bld->getType().compare("Settlement"))
            {
                if(player->getIsLocal()) bld->setIsSelectable(true);
                selectableObjectFound = true;
            }
        }
    }

    if(selectableObjectFound)
    {
        if(player->getIsLocal()) board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("Waiting for %1 to select a settlement.").
            arg(player->getName()))
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuildSettlement)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleCanBuildSettlement);
    RULECHAIN_ADD(ruleCanBuySettlement);
    RULECHAIN_ADD(ruleSetCancelable);
    RULECHAIN_ADD(ruleSelectCrossroad);
    RULECHAIN_ADD(ruleUnsetCancelable);
    RULECHAIN_ADD(ruleBuySettlement);
    RULECHAIN_ADD(ruleBuildSettlement);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleBuildSettlement)
{
    RULEDATA_REQUIRE("Crossroad");
    Crossroad *cr = RULEDATA_READ("Crossroad").value<Crossroad*>();

    Q_ASSERT(cr != NULL);
    Q_ASSERT(!cr->getIsPlayerObjectPlaced());

    // place object
    PlayerObject *bld = player->getUnplacedObjectOfType("Settlement");
    bld->setScale(0.3);
    cr->placePlayerObject(bld);
    player->increaseWinningPointsBy(1);

    EXECUTE_SUBRULE(ruleUpdateInterface);
    LOG_PLAYER_MSG(QString("%1 built a settlement.").arg(player->getName()));
    return true;
}

IMPLEMENT_RULE(ruleBuySettlement)
{
    GameCardStack *clayStack = game->getBank()->getCardStack("Clay");
    GameCardStack *lumberStack = game->getBank()->getCardStack("Lumber");
    GameCardStack *stack = player->getCardStack();

    stack->drawCardsOfType(clayStack, "Resource", "Clay", 2);
    stack->drawCardsOfType(lumberStack, "Resource", "Lumber", 3);

    LOG_PLAYER_MSG(QString("%1 payed 2 clay and 3 lumber.")
        .arg(player->getName()));

    return true;
}

IMPLEMENT_RULE(ruleCanBuildSettlement)
{
    // check for available settlement
    unsigned int n = player->getNumberOfUnplacedObjectsOfType("Settlement");
    if(n < 1) return false;

    return true;
}

IMPLEMENT_RULE(ruleCanBuySettlement)
{
    GameCardStack *stack = player->getCardStack();
    unsigned int clayCards = stack->getNumberOfCards("Resource", "Clay");
    unsigned int lumberCards = stack->getNumberOfCards("Resource", "Lumber");

    if(clayCards < 2 || lumberCards < 3) return false;

    return true;
}

IMPLEMENT_RULE(ruleRemoveSettlement)
{
    RULEDATA_REQUIRE("Settlement");
    PlayerObject *b = RULEDATA_POP("Settlement").value<PlayerObject*>();

    Q_ASSERT(b->getIsPlaced());
    b->getBaseObject()->placePlayerObject(NULL);
    b->setBaseObject(NULL);
    b->getOwner()->increaseWinningPointsBy(-1);

    return true;
}

// Lets the user select a crossroad
IMPLEMENT_RULE(ruleSelectCrossroad)
{
    Board *board = game->getBoard();
    const QList<Crossroad*> crossroads = board->getCrossroads();
    bool selectableObjectFound = false;

    // set all crossroads selectable
    for(int i = 0; i < crossroads.size(); i++)
    {
        RULEDATA_PUSH("Crossroad", crossroads.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectCrossroad);
        if(player->getIsLocal()) crossroads.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        if(player->getIsLocal()) board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("Waiting for %1 to select a crossroad.")
            .arg(player->getName()));
        return true;
    }

    return false;
}

// Check if a crossroad can be selected
// it cannot be selected if it has only water tiles around
// since these are the standard rules without seafarers
IMPLEMENT_RULE(ruleCanSelectCrossroad)
{
    RULEDATA_REQUIRE("Crossroad");
    Crossroad *c = RULEDATA_POP("Crossroad").value<Crossroad*>();

    if(c->getIsPlayerObjectPlaced()) return false;

    for(int i = 0; i < c->getNeighbours().size(); i++)
    {
        if(c->getNeighbours().at(i)->getIsPlayerObjectPlaced()) return false;
    }

    for(int i = 0; i < c->getTiles().size(); i++)
    {
        if(c->getTiles().at(i)->getType() != HexTile::WaterType)
            return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuildRoad)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleCanBuildRoad);
    RULECHAIN_ADD(ruleCanBuyRoad);
    RULECHAIN_ADD(ruleSetCancelable);
    RULECHAIN_ADD(ruleSelectRoadway);
    RULECHAIN_ADD(ruleUnsetCancelable);
    RULECHAIN_ADD(ruleBuyRoad);
    RULECHAIN_ADD(ruleBuildRoad);
    startRuleChain();
    return true;
}

IMPLEMENT_RULE(ruleBuildRoad)
{
    RULEDATA_REQUIRE("Roadway");
    Roadway *r = RULEDATA_POP("Roadway").value<Roadway*>();

    PlayerObject *road = player->getUnplacedObjectOfType("Road");
    road->setScale(0.3);
    r->placePlayerObject(road);
    
    EXECUTE_SUBRULE(ruleUpdateInterface);
    LOG_PLAYER_MSG(QString("%1 built a road.").arg(player->getName()));
    return true;
}

IMPLEMENT_RULE(ruleCanBuildRoad)
{
    unsigned int nR = player->getNumberOfUnplacedObjectsOfType("Road");
    if(nR < 1) return false;

    return true;
}

IMPLEMENT_RULE(ruleCanBuyRoad)
{
    GameCardStack *stack = player->getCardStack();
    unsigned int nClay = stack->getNumberOfCards("Resource", "Clay");
    unsigned int nLumber = stack->getNumberOfCards("Resource", "Lumber");

    if(nClay < 1 || nLumber < 1) return false;

    return true;
}

IMPLEMENT_RULE(ruleBuyRoad)
{
    GameCardStack *clayStack = game->getBank()->getCardStack("Clay");
    GameCardStack *lumberStack = game->getBank()->getCardStack("Lumber");
    GameCardStack *playerStack = player->getCardStack();

    playerStack->drawCardsOfType(clayStack, "Resource", "Clay", 1);
    playerStack->drawCardsOfType(lumberStack, "Resource", "Lumber", 1);

    LOG_PLAYER_MSG(QString("%1 payed 1 clay and 1 lumber")
        .arg(player->getName()));

    return true;
}

IMPLEMENT_RULE(ruleSelectRoadway)
{
    Board *board = game->getBoard();
    QList<Roadway*> roadways = board->getRoadways();
    bool selectableObjectFound = false;

    for(int i = 0; i < roadways.size(); i++)
    {
        RULEDATA_PUSH("Roadway", roadways.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectRoadway);
        if(player->getIsLocal()) roadways.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        if(player->getIsLocal()) board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("Waiting for %1 to select a roadway.")
            .arg(player->getName()));
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleSelectRoadwayAtCrossroad)
{
    RULEDATA_REQUIRE("Crossroad");
    Crossroad *cr = RULEDATA_POP("Crossroad").value<Crossroad*>();
    QList<Roadway*> roadways = cr->getRoadways();
    bool selectableObjectFound = false;
    Board *board = game->getBoard();

    for(int i = 0; i < roadways.size(); i++)
    {
        RULEDATA_PUSH("Roadway", roadways.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectRoadway);
        if(player->getIsLocal()) roadways.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        if(player->getIsLocal()) board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("Waiting for %1 to select a roadway.")
            .arg(player->getName()));
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleCanSelectRoadway)
{
    RULEDATA_REQUIRE("Roadway");

    Roadway *r = RULEDATA_POP("Roadway").value<Roadway*>();
    QList<Crossroad*> crossroads = r->getCrossroads();
    QList<Roadway*> roadways = r->getNeighbours();
    bool canSelect = false;

    // find crossroads, the player has a building on
    for(int i = 0; i < crossroads.size(); i++)
    {
        Crossroad *c = crossroads.at(i);
        if(c->getIsPlayerObjectPlaced() &&
            c->getPlayerObject()->getOwner() == player) canSelect = true;
    }

    // is a neighbour road ours?
    for(int i = 0; i < roadways.size(); i++)
    {
        Roadway *rw = roadways.at(i);
        if(rw->getIsPlayerObjectPlaced() &&
            rw->getPlayerObject()->getOwner() == player) canSelect = true;
    }

    // we cannot build roads at water tiles
    // (these are the standard rules without seafarers)
    for(int i = 0; i < r->getTiles().size(); i++)
    {
        if(r->getTiles().at(i)->getType() != HexTile::WaterType)
            return canSelect;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuyDevelopmentCard)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(ruleCanBuyDevelopmentCard);
    RULECHAIN_ADD(ruleBuyDevelopmentCard);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleCanBuyDevelopmentCard)
{
    // is a card available?
    GameCardStack *devCardStack = game->getBank()->getCardStack("Development");
    if(devCardStack->getNumberOfCards() < 1) return false;

    // check resources
    GameCardStack *pStack = player->getCardStack();
    unsigned int nWheat = pStack->getNumberOfCards("Resource", "Wheat");
    unsigned int nOre = pStack->getNumberOfCards("Resource", "Ore");

    if(nWheat < 1 || nOre < 1) return false;

    return true;
}

IMPLEMENT_RULE(ruleBuyDevelopmentCard)
{
    GameCardStack *devCardStack = game->getBank()->getCardStack("Development");
    GameCardStack *wheatStack = game->getBank()->getCardStack("Wheat");
    GameCardStack *oreStack = game->getBank()->getCardStack("Ore");
    GameCardStack *pStack = player->getCardStack();

    pStack->drawCardsOfType(wheatStack, "Resource", "Wheat", 1);
    pStack->drawCardsOfType(oreStack, "Resource", "Ore", 1);

    devCardStack->drawFirstCards(pStack, 1);

    LOG_PLAYER_MSG(QString("%1 bought a development card.")
        .arg(player->getName()));

    EXECUTE_SUBRULE(ruleUpdateInterface);

    return true;
}

IMPLEMENT_RULE(ruleCanPlayKnightCard)
{
    if(player != currentPlayer) return false;

    return true;
}

IMPLEMENT_RULE(ruleCanPlayBuildRoadCard)
{
    if(!diceRolled) return false;
    if(player != currentPlayer) return false;

    return true;
}

IMPLEMENT_RULE(ruleUserActionPlayBuildRoadCard)
{
    SERVER_ONLY_RULE

    pushRuleChain();
    RULECHAIN_ADD(rulePlayBuildRoadCard);
    RULECHAIN_ADD(ruleCanBuildRoad);
    RULECHAIN_ADD(ruleSetCancelable);
    RULECHAIN_ADD(ruleSelectRoadway);
    RULECHAIN_ADD(ruleUnsetCancelable);
    RULECHAIN_ADD(ruleBuildRoad);
    RULECHAIN_ADD(ruleCanBuildRoad);
    RULECHAIN_ADD(ruleSetCancelable);
    RULECHAIN_ADD(ruleSelectRoadway);
    RULECHAIN_ADD(ruleUnsetCancelable);
    RULECHAIN_ADD(ruleBuildRoad);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(rulePlayBuildRoadCard)
{
    GameCardStack *ps = player->getCardStack();
    GameCardStack *dc = game->getBank()->getCardStack("Discarded");

    ps->drawCardsOfType(dc, "Development", "Build Road", 1);

    LOG_PLAYER_MSG(QString("%1 plays a Build Road card.")
        .arg(player->getName()));

    EXECUTE_SUBRULE(ruleUpdateInterface);

    return true;
}

IMPLEMENT_RULE(ruleCanPlayMonopolyCard)
{
    if(!diceRolled) return false;
    if(player != currentPlayer) return false;

    return true;
}

IMPLEMENT_RULE(ruleCanPlayInventionCard)
{
    if(!diceRolled) return false;
    if(player != currentPlayer) return false;

    return true;
}

IMPLEMENT_RULE(ruleCanPlayWinningPointCard)
{
    return true;
}

