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
#include "FileManager.h"
#include "NetworkPacket.h"
#include "NetworkCore.h"
#include "GameLobby.h"
#include "Game.h"
#include "StandardMap.h"
#include "GameRules.h"

GameRules::GameRules(QObject *parent) : QObject(parent)
{
    isRuleChainWaiting = false;
    playerPanel = NULL;
    gameInfoPanel = NULL;
    messagePanel = NULL;
    controlPanel = NULL;

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
    REGISTER_RULE(ruleDrawCardsFromBankStack);
    REGISTER_RULE(ruleInitDockWidgets);
    REGISTER_RULE(ruleInitPlayerPanel);
    REGISTER_RULE(ruleUpdatePlayerPanel);
    REGISTER_RULE(ruleInitControlPanel);
    REGISTER_RULE(ruleUpdateControlPanel);
    REGISTER_RULE(ruleGenerateBoard);
    REGISTER_RULE(ruleUpdateInterface);
    REGISTER_RULE(ruleBoardObjectSelected);
    REGISTER_RULE(ruleBuildCity);
    REGISTER_RULE(ruleCanBuildCity);
    REGISTER_RULE(ruleSelectSettlement);
    REGISTER_RULE(ruleSettlementSelected);
    REGISTER_RULE(ruleUserActionBuildSettlement);
    REGISTER_RULE(ruleBuildSettlement);
    REGISTER_RULE(ruleCanBuildSettlement);
    REGISTER_RULE(ruleRemoveSettlement);
    REGISTER_RULE(ruleSelectCrossroad);
    REGISTER_RULE(ruleCanSelectCrossroad);
    REGISTER_RULE(ruleUserActionBuildRoad);
    REGISTER_RULE(ruleBuildRoad);
    REGISTER_RULE(ruleCanBuildRoad);
    REGISTER_RULE(ruleSelectRoadway);
    REGISTER_RULE(ruleSelectRoadwayAtCrossroad);
    REGISTER_RULE(ruleCanSelectRoadway);
}

GameRules::~GameRules()
{
    reset();
}

void GameRules::reset()
{
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

        else if(!qstrcmp(typeName, "GLGameModel*"))
        { v = qVariantFromValue(GLGameModelPtr(i.value().value<GLGameModel*>())); }

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

        else if(!qstrcmp(typeName, "GLGameModelPtr"))
        { v = qVariantFromValue(i.value().value<GLGameModelPtr>().getObject()); }

        qDebug() << "Unpacking rule data" << i.key() << v;
        ruleData.insertMulti(i.key(), v);
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
    qDebug() << "Trying to execute local player rule:" << name;
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

bool GameRules::executeSubRule(QString name, Player *player)
{
    Q_ASSERT(player != NULL);
    Q_ASSERT(rules.contains(name));

    qDebug() << "Executing nested rule" << name << player->getName();
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
        if(!executeRule(rce.name, rce.player)) cancelRuleChain();
        if(isRuleChainWaiting) break; // detect nested suspensions
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
    PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(GLGameModel);
    PUSH_TO_RULEDATA_IF_QOBJECT_TYPE(Player);

    qDebug() << "Unknown type!";
    Q_ASSERT(false);
}

void GameRules::pushRuleData(const QString &identifier, QVariant data)
{
    ruleData.insertMulti(identifier, data);
}

// STANDARD RULES

IMPLEMENT_RULE(ruleStartServer)
{
    game->getNetworkCore()->startServer(1234);
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
    game->getLobby()->hide();
    EXECUTE_SUBRULE(ruleInitGame);
    game->setState(Game::PlayingState);
    return true;
}

IMPLEMENT_RULE(ruleInitGame)
{
    SERVER_ONLY_RULE

    RULECHAIN_ADD(ruleInitGameCards);
    RULECHAIN_ADD(ruleInitPlayers);
    RULECHAIN_ADD(ruleInitDockWidgets);
    RULECHAIN_ADD(ruleInitPlayerPanel);
    RULECHAIN_ADD(ruleInitControlPanel);
    RULECHAIN_ADD(ruleGenerateBoard);
    RULECHAIN_ADD(ruleInitialPlacement);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleInitialPlacement1)
{
    SERVER_ONLY_RULE

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
    return true;
}

IMPLEMENT_RULE(ruleEndTurn)
{
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;
    Q_ASSERT(players.contains(player));

    i += players.indexOf(player) + 1;

    if(i == players.end())
    { player = players.at(0); }
    else
    { player = *i; }

    LOG_PLAYER_MSG(QString("%1 finished turn.").arg(player->getName()));

    return EXECUTE_SUBRULE(ruleBeginTurn);
}

IMPLEMENT_RULE(ruleDrawInitialResourceCards)
{
    RULEDATA_REQUIRE("Crossroad");

    Crossroad *r = RULEDATA_READ("Crossroad").value<Crossroad*>();
    RULEDATA_PUSH("Amount", 1);

    for(int i = 0; i < r->getTiles().size(); i++)
    {
        QString stack;

        switch(r->getTiles().at(i)->getType())
        {
            case HexTile::HexTileTypeOre:   stack = "Ore";    break;
            case HexTile::HexTileTypeClay:  stack = "Clay";   break;
            case HexTile::HexTileTypeWheat: stack = "Wheat";  break;
            case HexTile::HexTileTypeSheep: stack = "Sheep";  break;
            case HexTile::HexTileTypeWood:  stack = "Lumber"; break;
            default: continue;
        }

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

    LOG_PLAYER_MSG(QString("%1 draws a %2 card from the bank.")
        .arg(player->getName()).arg(stackName));

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

    // resources
    bank->registerCardStack("Wheat");
    bank->registerCardStack("Sheep");
    bank->registerCardStack("Ore");
    bank->registerCardStack("Lumber");
    bank->registerCardStack("Clay");

    bank->registerCard("Wheat",  GAMECARD_WHEAT,  20);
    bank->registerCard("Sheep",  GAMECARD_SHEEP,  20);
    bank->registerCard("Ore",    GAMECARD_ORE,    20);
    bank->registerCard("Lumber", GAMECARD_LUMBER, 20);
    bank->registerCard("Clay",   GAMECARD_CLAY,   20);

    // development cards
    bank->registerCardStack("Development");
    bank->registerCard("Development", GAMECARD_KNIGHT, 10);
    bank->registerCard("Development", GAMECARD_BUILD_ROAD, 4);
    bank->getCardStack("Development")->shuffle();

    return true;
}

IMPLEMENT_RULE(ruleInitDockWidgets)
{
    QMainWindow *mainWindow = game->getMainWindow();

    playerPanel = new PlayerPanel("", mainWindow);
    playerPanel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    playerPanel->setFixedWidth(200);
    playerPanel->setFloating(false);
    playerPanel->setFeatures(QDockWidget::DockWidgetMovable);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, playerPanel);

    gameInfoPanel = new GameInfoPanel("Game Info", mainWindow);
    gameInfoPanel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    gameInfoPanel->setMinimumHeight(150);
    gameInfoPanel->setMaximumHeight(150);
    gameInfoPanel->setFeatures(QDockWidget::DockWidgetMovable);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, gameInfoPanel);

    messagePanel = new MessagePanel("Messages", mainWindow);
    messagePanel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    messagePanel->setMinimumHeight(250);
    messagePanel->setMinimumWidth(250);
    messagePanel->setMaximumWidth(500);
    messagePanel->setFeatures(QDockWidget::DockWidgetMovable);
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, messagePanel);

    controlPanel = new ControlPanel("", mainWindow);
    controlPanel->setFixedHeight(60);
    controlPanel->setMaximumHeight(60);
    controlPanel->setMinimumHeight(60);
    controlPanel->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    controlPanel->setAllowedAreas(Qt::TopDockWidgetArea);
    mainWindow->addDockWidget(Qt::TopDockWidgetArea, controlPanel);

    return true;
}

IMPLEMENT_RULE(ruleInitPlayerPanel)
{
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;

    Q_ASSERT(players.size() > 0);

    for(i = players.begin(); i != players.end(); ++i)
    {
        playerPanel->registerPlayerInfo(*i, "WinningPoints", "Winning Points",
            "WinningPoints.png");
        playerPanel->registerPlayerInfo(*i, "Roads", "Available Roads",
            "Road.png", true);
        playerPanel->registerPlayerInfo(*i, "Settlements", "Available Settlements",
            "Settlement.png", true);
        playerPanel->registerPlayerInfo(*i, "Cities", "Available Cities",
            "City.png", true);
        playerPanel->registerPlayerInfo(*i, "DevelopmentCards", "Development Cards",
            "DevelopmentCard.png");

        if(((Player*)*i)->getIsLocal())
        {
            playerPanel->registerPlayerInfo(*i, "Wheat", "Wheat", "Wheat.png");
            playerPanel->registerPlayerInfo(*i, "Lumber", "Lumber", "Lumber.png");
            playerPanel->registerPlayerInfo(*i, "Sheep", "Sheep", "Sheep.png");
            playerPanel->registerPlayerInfo(*i, "Ore", "Ore", "Ore.png");
            playerPanel->registerPlayerInfo(*i, "Clay", "Clay", "Clay.png");
        }
        else
        {
            playerPanel->registerPlayerInfo(*i, "ResourceCards", "Resource Cards",
                "Card.png");
        }
        
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

    if(player->getIsLocal())
    {
        playerPanel->updatePlayerInfo(player, "Wheat",
            player->getCardStack()->getNumberOfCards("Resource", "Wheat"));
        playerPanel->updatePlayerInfo(player, "Lumber",
            player->getCardStack()->getNumberOfCards("Resource", "Lumber"));
        playerPanel->updatePlayerInfo(player, "Sheep",
            player->getCardStack()->getNumberOfCards("Resource", "Sheep"));
        playerPanel->updatePlayerInfo(player, "Ore",
            player->getCardStack()->getNumberOfCards("Resource", "Ore"));
        playerPanel->updatePlayerInfo(player, "Clay",
            player->getCardStack()->getNumberOfCards("Resource", "Clay"));
    }
    else
    {
        playerPanel->updatePlayerInfo(player, "ResourceCards",
            player->getCardStack()->getNumberOfCards("Resource"));
    }

    return true;
}

IMPLEMENT_RULE(ruleInitControlPanel)
{
    QAction *actionBuildRoad = new QAction(controlPanel);
    actionBuildRoad->setData(QString("ruleUserActionBuildRoad"));
    actionBuildRoad->setToolTip("Build Road");
    actionBuildRoad->setIcon(QIcon(FileManager::getPathOfImage("Road.png")));
    controlPanel->registerAction("BuildRoad", actionBuildRoad);

    QAction *actionBuildSettlement = new QAction(controlPanel);
    actionBuildSettlement->setData(QString("ruleUserActionBuildSettlement"));
    actionBuildSettlement->setToolTip("Build Settlement");
    actionBuildSettlement->setIcon(QIcon(FileManager::getPathOfImage("Settlement.png")));
    controlPanel->registerAction("BuildSettlement", actionBuildSettlement);

    QAction *actionBuildCity = new QAction(controlPanel);
    actionBuildCity->setData(QString("ruleUserActionBuildCity"));
    actionBuildCity->setToolTip("Build City");
    actionBuildCity->setIcon(QIcon(FileManager::getPathOfImage("City.png")));
    controlPanel->registerAction("BuildCity", actionBuildCity);

    QAction *actionShowCards = new QAction(controlPanel);
    actionShowCards->setData(QString("ruleUserActionShowCards"));
    actionShowCards->setToolTip("Show my cards");
    actionShowCards->setIcon(QIcon(FileManager::getPathOfImage("Cards.png")));
    controlPanel->registerAction("ShowCards", actionShowCards);

    QAction *actionBuyDevCard = new QAction(controlPanel);
    actionBuyDevCard->setData(QString("ruleUserActionBuyDevelopmentCard"));
    actionBuyDevCard->setToolTip("Buy Development Card");
    actionBuyDevCard->setIcon(QIcon(FileManager::getPathOfImage("BuyDevCard.png")));
    controlPanel->registerAction("BuyDevCard", actionBuyDevCard);

    QAction *actionTrade = new QAction(controlPanel);
    actionTrade->setData(QString("ruleUserActionTrade"));
    actionTrade->setToolTip("Trade");
    actionTrade->setIcon(QIcon(FileManager::getPathOfImage("Trade.png")));
    controlPanel->registerAction("Trade", actionTrade);

    QAction *actionRollDice = new QAction(controlPanel);
    actionRollDice->setData(QString("ruleUserActionRollDice"));
    actionRollDice->setToolTip("Roll Dice");
    actionRollDice->setIcon(QIcon(FileManager::getPathOfImage("Dice.png")));
    controlPanel->registerAction("RollDice", actionRollDice);

    QAction *actionEndTurn = new QAction(controlPanel);
    actionEndTurn->setData(QString("ruleUserActionEndTurn"));
    actionEndTurn->setToolTip("End Turn");
    actionEndTurn->setIcon(QIcon(FileManager::getPathOfImage("EndTurn.png")));
    controlPanel->registerAction("EndTurn", actionEndTurn);

    EXECUTE_SUBRULE(ruleUpdateControlPanel);

    return true;
}

IMPLEMENT_RULE(ruleUpdateControlPanel)
{
    if(!player->getIsLocal()) return true;

    controlPanel->setActionState("BuildRoad", false);
    controlPanel->setActionState("BuildSettlement", false);
    controlPanel->setActionState("BuildCity", false);
    controlPanel->setActionState("BuyDevCard", false);
    controlPanel->setActionState("ShowCards", true);
    controlPanel->setActionState("Trade", false);
    controlPanel->setActionState("RollDice", false);
    controlPanel->setActionState("EndTurn", false);

    return true;
}

IMPLEMENT_RULE(ruleUpdateInterface)
{
    EXECUTE_SUBRULE(ruleUpdatePlayerPanel);
    EXECUTE_SUBRULE(ruleUpdateControlPanel);

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

IMPLEMENT_RULE(ruleUserActionBuildCity)
{
    SERVER_ONLY_RULE

    RULECHAIN_ADD(ruleCanBuildCity);
    RULECHAIN_ADD(ruleSelectSettlement);
    RULECHAIN_ADD(ruleSettlementSelected);
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

IMPLEMENT_RULE(ruleSettlementSelected)
{
    LOCAL_ONLY_RULE

    Board *board = game->getBoard();
    if(board->getHasSelectedObject())
    {
        RULEDATA_PUSH("Crossroad", (Crossroad*)board->getSelectedObject());
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuildSettlement)
{
    SERVER_ONLY_RULE

    RULECHAIN_ADD(ruleCanBuildSettlement);
    RULECHAIN_ADD(ruleSelectCrossroad);
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

    PlayerObject *bld = player->getUnplacedObjectOfType("Settlement");
    bld->setScale(0.3);
    cr->placePlayerObject(bld);
    player->increaseWinningPointsBy(1);

    EXECUTE_SUBRULE(ruleUpdateInterface);
    LOG_PLAYER_MSG(QString("%1 built a settlement.").arg(player->getName()));
    return true;
}

IMPLEMENT_RULE(ruleCanBuildSettlement)
{
    return true;
}

IMPLEMENT_RULE(ruleRemoveSettlement)
{
    RULEDATA_REQUIRE("Settlement");
    PlayerObject *b = RULEDATA_POP("Settlement").value<PlayerObject*>();

    Q_ASSERT(b->getIsPlaced());
    Q_ASSERT(b->getBaseObject() == (GLGameModelProxy*)b);
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
        if(c->getTiles().at(i)->getType() != HexTile::HexTileTypeWater)
            return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuildRoad)
{
    SERVER_ONLY_RULE

    RULECHAIN_ADD(ruleCanBuildRoad);
    RULECHAIN_ADD(ruleSelectRoadway);
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
    bool canSelect = false;

    for(int i = 0; i < crossroads.size(); i++)
    {
        if(crossroads.at(i)->getIsPlayerObjectPlaced()) canSelect = true;
    }

    for(int i = 0; i < r->getTiles().size(); i++)
    {
        if(r->getTiles().at(i)->getType() != HexTile::HexTileTypeWater)
            return canSelect;
    }

    return false;
}

