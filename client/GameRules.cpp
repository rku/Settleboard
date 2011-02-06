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
#include "Game.h"

GameRules::GameRules(QObject *parent) : QObject(parent)
{
    isRuleChainWaiting = false;
    playerPanel = NULL;
    gameInfoPanel = NULL;
    messagePanel = NULL;
    controlPanel = NULL;

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

    REGISTER_RULE(ruleUserActionBuildCity);
    REGISTER_RULE(ruleBuildCity);
    REGISTER_RULE(ruleCanBuildCity);

    REGISTER_RULE(ruleSelectSettlement);
    REGISTER_RULE(ruleSettlementSelected);
    REGISTER_RULE(ruleUserActionBuildSettlement);
    REGISTER_RULE(ruleBuildSettlement);
    REGISTER_RULE(ruleCanBuildSettlement);
    REGISTER_RULE(ruleRemoveSettlement);

    REGISTER_RULE(ruleSelectCrossroad);
    REGISTER_RULE(ruleCrossroadSelected);
    REGISTER_RULE(ruleCanSelectCrossroad);

    REGISTER_RULE(ruleUserActionBuildRoad);
    REGISTER_RULE(ruleBuildRoad);
    REGISTER_RULE(ruleCanBuildRoad);

    REGISTER_RULE(ruleSelectRoadway);
    REGISTER_RULE(ruleSelectRoadwayAtCrossroad);
    REGISTER_RULE(ruleRoadwaySelected);
    REGISTER_RULE(ruleCanSelectRoadway);
}

GameRules::~GameRules()
{
}

void GameRules::registerRule(QString name, GameRule rule)
{
    // consider registering a rule twice an error
    Q_ASSERT(!rules.contains(name));
    rules.insert(name, rule);

    qDebug() << "Rule registered:" << name;
}

bool GameRules::executeRule(QString name, Player *player)
{
    Q_ASSERT(!isRuleChainWaiting);

    qDebug() << "Executing rule:" << name;
    Q_ASSERT(rules.contains(name));

    GameRule rule = rules.value(name);
    return (this->*rule.ruleFunc)(Game::getInstance(), player);
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
    Q_ASSERT(ruleChain.size() > 0 && !isRuleChainWaiting);
    qDebug() << "Starting rule chain";

    continueRuleChain();
}

void GameRules::continueRuleChain()
{
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
    Game::getInstance()->getBoard()->update();
}

void GameRules::ruleChainFinished()
{
    qDebug() << "Rule chain finished";
    ruleChain.clear();
    ruleData.clear();
    isRuleChainWaiting = false;
    Game::getInstance()->getBoard()->resetBoardState();
}

void GameRules::cancelRuleChain()
{
    ruleData.clear();
    ruleChain.clear();
    isRuleChainWaiting = false;

    qDebug() << "Rule chain canceled";
}

// STANDARD RULES

IMPLEMENT_RULE(ruleInitGame)
{
    player = game->getPlayers()[0];

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
    RULECHAIN_ADD(ruleSelectCrossroad);
    RULECHAIN_ADD(ruleCrossroadSelected);
    RULECHAIN_ADD(ruleBuildSettlement);
    RULECHAIN_ADD(ruleSelectRoadwayAtCrossroad);
    RULECHAIN_ADD(ruleRoadwaySelected);
    RULECHAIN_ADD(ruleBuildRoad);
    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleInitialPlacement2)
{
    RULECHAIN_ADD(ruleSelectCrossroad);
    RULECHAIN_ADD(ruleCrossroadSelected);
    RULECHAIN_ADD(ruleDrawInitialResourceCards);
    RULECHAIN_ADD(ruleBuildSettlement);
    RULECHAIN_ADD(ruleSelectRoadwayAtCrossroad);
    RULECHAIN_ADD(ruleRoadwaySelected);
    RULECHAIN_ADD(ruleBuildRoad);

    startRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleInitialPlacement)
{
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
            case HEXTILE_TYPE_ORE:   stack = "Ore";    break;
            case HEXTILE_TYPE_CLAY:  stack = "Clay";   break;
            case HEXTILE_TYPE_WHEAT: stack = "Wheat";  break;
            case HEXTILE_TYPE_SHEEP: stack = "Sheep";  break;
            case HEXTILE_TYPE_WOOD:  stack = "Lumber"; break;
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

        for(int i = 0; i < 10; i++) p->addObjectOfType("Road");
        for(int i = 0; i < 5;  i++) p->addObjectOfType("Settlement");
        for(int i = 0; i < 5;  i++) p->addObjectOfType("City");

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
    messagePanel->setMinimumHeight(150);
    messagePanel->setMinimumWidth(150);
    messagePanel->setMaximumWidth(400);
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
    return true;
}

IMPLEMENT_RULE(ruleGenerateBoard)
{
    game->getBoard()->loadByName("StandardSettlers");
    game->getBoard()->generate();
    game->getBoard()->update();
    return true;
}

IMPLEMENT_RULE(ruleUserActionBuildCity)
{
    if(player->getIsLocal())
    {
        RULECHAIN_ADD(ruleCanBuildCity);
        RULECHAIN_ADD(ruleSelectSettlement);
        RULECHAIN_ADD(ruleSettlementSelected);
        RULECHAIN_ADD(ruleBuildCity);
        
        startRuleChain();
    }

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
    if(player->getIsLocal())
    {
        RULECHAIN_ADD(ruleCanBuildSettlement);
        RULECHAIN_ADD(ruleSelectCrossroad);
        RULECHAIN_ADD(ruleCrossroadSelected);
        RULECHAIN_ADD(ruleBuildSettlement);

        startRuleChain();
    }

    return true;
}

IMPLEMENT_RULE(ruleBuildSettlement)
{
    RULEDATA_REQUIRE("Crossroad");

    Crossroad *cr = RULEDATA_READ("Crossroad").value<Crossroad*>();

    Q_ASSERT(!cr->getIsPlayerObjectPlaced());
    PlayerObject *bld = player->getUnplacedObjectOfType("Settlement");
    bld->setScale(0.3);
    cr->placePlayerObject(bld);

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

IMPLEMENT_RULE(ruleCrossroadSelected)
{
    Board *board = game->getBoard();
    Crossroad *obj = (Crossroad*)board->getSelectedObject();
    Q_ASSERT(obj != NULL);

    if(board->getHasSelectedObject())
    {
        RULEDATA_PUSH("Crossroad", obj);
        return true;
    }

    return false;
}

// Check if a crossroad can be selected
// it cannot be selected if it has only water tiles around
// since these are the standard rules without seafarers
IMPLEMENT_RULE(ruleCanSelectCrossroad)
{
    if(!player->getIsLocal()) return true;

    RULEDATA_REQUIRE("Crossroad");

    Crossroad *c = RULEDATA_POP("Crossroad").value<Crossroad*>();

    if(c->getIsPlayerObjectPlaced()) return false;

    for(int i = 0; i < c->getNeighbours().size(); i++)
    {
        if(c->getNeighbours().at(i)->getIsPlayerObjectPlaced()) return false;
    }

    for(int i = 0; i < c->getTiles().size(); i++)
    {
        if(c->getTiles().at(i)->getType() != HEXTILE_TYPE_WATER)
            return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuildRoad)
{
    if(player->getIsLocal())
    {
        RULECHAIN_ADD(ruleCanBuildRoad);
        RULECHAIN_ADD(ruleSelectRoadway);
        RULECHAIN_ADD(ruleRoadwaySelected);
        RULECHAIN_ADD(ruleBuildRoad);

        startRuleChain();
    }

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
    if(!player->getIsLocal())
    {
        LOG_PLAYER_MSG(QString("Waiting for %1 to select a roadway.")
            .arg(player->getName()));
        return true;
    }

    RULEDATA_REQUIRE("Crossroad");
    Crossroad *cr = RULEDATA_READ("Crossroad").value<Crossroad*>();
    QList<Roadway*> roadways = cr->getRoadways();
    bool selectableObjectFound = false;
    Board *board = game->getBoard();

    for(int i = 0; i < roadways.size(); i++)
    {
        RULEDATA_PUSH("Roadway", roadways.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectRoadway);
        roadways.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        board->setSelectionMode();
        suspendRuleChain();
        LOG_SYSTEM_MSG(QString("%1, please select a roadway.")
            .arg(player->getName()));
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleRoadwaySelected)
{
    Board *board = game->getBoard();

    if(board->getHasSelectedObject())
    {
        RULEDATA_PUSH("Roadway", (Roadway*)board->getSelectedObject());
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
        if(r->getTiles().at(i)->getType() != HEXTILE_TYPE_WATER)
            return canSelect;
    }

    return false;
}

