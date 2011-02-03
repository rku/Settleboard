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
#include "Game.h"

GameRules::GameRules(Game *_game)
    : GameObject(_game)
{
    isRuleChainWaiting = false;

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

    REGISTER_RULE(ruleInitPlayerPanel);
    REGISTER_RULE(ruleUpdatePlayerPanel);
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

    initActions();
}

GameRules::~GameRules()
{
    while(!actions.isEmpty()) delete actions.takeFirst();
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
    return (this->*rule.ruleFunc)(rule, player);
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
    game->getBoard()->update();
}

void GameRules::ruleChainFinished()
{
    qDebug() << "Rule chain finished";
    ruleChain.clear();
    ruleData.clear();
    isRuleChainWaiting = false;
    game->getBoard()->resetBoardState();
}

void GameRules::cancelRuleChain()
{
    ruleData.clear();
    ruleChain.clear();
    isRuleChainWaiting = false;

    qDebug() << "Rule chain canceled";
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

unsigned int GameRules::getWinningPoints()
{
    return winningPoints;
}

void GameRules::setWinningPoints(unsigned int n)
{
    Q_ASSERT(n > 0);
    winningPoints = n;
}

// STANDARD RULES

IMPLEMENT_RULE(ruleInitGame)
{
    player = game->getPlayers()[0];

    RULECHAIN_ADD(ruleInitGameCards);
    RULECHAIN_ADD(ruleInitPlayers);
    RULECHAIN_ADD(ruleInitPlayerPanel);
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

    return EXECUTE_SUBRULE(ruleBeginTurn);
}

IMPLEMENT_RULE(ruleDrawInitialResourceCards)
{
    RULEDATA_REQUIRE("Crossroad");

    Crossroad *r = (Crossroad*)RULEDATA_READ_POINTER("Crossroad");
    RULEDATA_PUSH_UINT("Amount", 1);

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

        RULEDATA_PUSH_STRING("CardStackName", stack);
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
    QString stackName = RULEDATA_POP_STRING("CardStackName");
    uint amount = RULEDATA_READ_UINT("Amount");
    GameCardStack *stack = bank->getCardStack(stackName);

    qDebug() << "Player" << player->getName() << "draws" << amount
        << "cards from bank stack" << stackName;

    bool r = stack->drawFirstCards(player->getCardStack(), amount);
    EXECUTE_SUBRULE(ruleUpdateInterface);
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

IMPLEMENT_RULE(ruleInitPlayerPanel)
{
    PlayerPanel *panel = game->getMainWindow()->getPlayerPanel();
    QList<Player*> players = game->getPlayers();
    QList<Player*>::iterator i;

    Q_ASSERT(players.size() > 0);

    for(i = players.begin(); i != players.end(); ++i)
    {
        panel->registerPlayerInfo(*i, "Roads", "Available Roads", "Road.png");
        panel->registerPlayerInfo(*i, "Settlements", "Available Settlements",
            "Settlement.png");
        panel->registerPlayerInfo(*i, "Cities", "Available Cities", "City.png");
        panel->registerPlayerInfo(*i, "DevelopmentCards", "Development Cards",
            "DevelopmentCard.png");

        if(((Player*)*i)->getIsLocal())
        {
            panel->registerPlayerInfo(*i, "Wheat", "Wheat", "Wheat.png");
            panel->registerPlayerInfo(*i, "Lumber", "Lumber", "Lumber.png");
            panel->registerPlayerInfo(*i, "Sheep", "Sheep", "Sheep.png");
            panel->registerPlayerInfo(*i, "Ore", "Ore", "Ore.png");
            panel->registerPlayerInfo(*i, "Clay", "Clay", "Clay.png");
        }
        else
        {
            panel->registerPlayerInfo(*i, "ResourceCards", "Resource Cards",
                "Card.png");
        }
        
        EXECUTE_SUBRULE_FOR_PLAYER(ruleUpdatePlayerPanel, *i);
    }

    return true;
}

IMPLEMENT_RULE(ruleUpdatePlayerPanel)
{
    PlayerPanel *panel = game->getMainWindow()->getPlayerPanel();

    panel->updatePlayerInfo(player, "Roads",
        player->getNumberOfUnplacedObjectsOfType("Road"));
    panel->updatePlayerInfo(player, "Settlements",
        player->getNumberOfUnplacedObjectsOfType("Settlement"));
    panel->updatePlayerInfo(player, "Cities",
        player->getNumberOfUnplacedObjectsOfType("City"));
    panel->updatePlayerInfo(player, "DevelopmentCards",
        player->getCardStack()->getNumberOfCards("Development"));

    if(player->getIsLocal())
    {
        panel->updatePlayerInfo(player, "Wheat",
            player->getCardStack()->getNumberOfCards("Resource", "Wheat"));
        panel->updatePlayerInfo(player, "Lumber",
            player->getCardStack()->getNumberOfCards("Resource", "Lumber"));
        panel->updatePlayerInfo(player, "Sheep",
            player->getCardStack()->getNumberOfCards("Resource", "Sheep"));
        panel->updatePlayerInfo(player, "Ore",
            player->getCardStack()->getNumberOfCards("Resource", "Ore"));
        panel->updatePlayerInfo(player, "Clay",
            player->getCardStack()->getNumberOfCards("Resource", "Clay"));
    }
    else
    {
        panel->updatePlayerInfo(player, "ResourceCards",
            player->getCardStack()->getNumberOfCards("Resource"));
    }

    return true;
}

IMPLEMENT_RULE(ruleUpdateInterface)
{
    EXECUTE_SUBRULE(ruleUpdatePlayerPanel);
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

    Crossroad *cr = (Crossroad*)RULEDATA_POP_POINTER("Crossroad");

    // remove settlement
    Q_ASSERT(cr->getIsPlayerObjectPlaced());
    RULEDATA_PUSH_POINTER("Settlement", cr->getPlayerObject());
    if(EXECUTE_SUBRULE(ruleRemoveSettlement))
    {
        // build city
        PlayerObject *bld = player->getUnplacedObjectOfType("City");
        bld->setScale(0.7);
        cr->placePlayerObject(bld);

        EXECUTE_SUBRULE(ruleUpdateInterface);
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
    Board *board = game->getBoard();
    QList<Crossroad*> crossroads = board->getCrossroads();
    bool selectableObjectFound = false;

    for(int i = 0; i < crossroads.size(); i++)
    {
        Crossroad *c = crossroads.at(i);
        if(c->getIsPlayerObjectPlaced())
        {
            PlayerObject *bld = (PlayerObject*)c->getPlayerObject();
            if(!bld->getType().compare("Settlement"))
            {
                bld->setIsSelectable(true);
                selectableObjectFound = true;
            }
        }
    }

    if(selectableObjectFound)
    {
        board->setSelectionMode();
        suspendRuleChain();
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleSettlementSelected)
{
    Board *board = game->getBoard();

    if(board->getHasSelectedObject())
    {
        RULEDATA_PUSH_POINTER("Crossroad", board->getSelectedObject());
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

    Crossroad *cr = (Crossroad*)RULEDATA_READ_POINTER("Crossroad");

    Q_ASSERT(!cr->getIsPlayerObjectPlaced());
    PlayerObject *bld = player->getUnplacedObjectOfType("Settlement");
    bld->setScale(0.3);
    cr->placePlayerObject(bld);

    EXECUTE_SUBRULE(ruleUpdateInterface);
    return true;
}

IMPLEMENT_RULE(ruleCanBuildSettlement)
{
    return true;
}

IMPLEMENT_RULE(ruleRemoveSettlement)
{
    RULEDATA_REQUIRE("Settlement");

    PlayerObject *b = (PlayerObject*)RULEDATA_POP_POINTER("Settlement");

    Q_ASSERT(b->getIsPlaced());
    Q_ASSERT(b->getBaseObject() == (GLGameModelProxy*)b);
    b->getBaseObject()->placePlayerObject(NULL);
    b->setBaseObject(NULL);

    return true;
}

// Lets the user select a crossroad
IMPLEMENT_RULE(ruleSelectCrossroad)
{
    if(!player->getIsLocal()) return true;

    Board *board = game->getBoard();
    const QList<Crossroad*> crossroads = board->getCrossroads();
    bool selectableObjectFound = false;

    // set all crossroads selectable
    for(int i = 0; i < crossroads.size(); i++)
    {
        RULEDATA_PUSH_POINTER("Crossroad", crossroads.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectCrossroad);
        crossroads.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        board->setSelectionMode();
        suspendRuleChain();
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleCrossroadSelected)
{
    Board *board = game->getBoard();
    GLGameModel *obj = board->getSelectedObject();
    Q_ASSERT(obj != NULL);

    if(board->getHasSelectedObject())
    {
        RULEDATA_PUSH_POINTER("Crossroad", obj);
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

    Crossroad *c = (Crossroad*)RULEDATA_POP_POINTER("Crossroad");

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

    Roadway *r = (Roadway*)RULEDATA_POP_POINTER("Roadway");
    PlayerObject *road = player->getUnplacedObjectOfType("Road");
    road->setScale(0.3);
    r->placePlayerObject(road);
    
    EXECUTE_SUBRULE(ruleUpdateInterface);
    return true;
}

IMPLEMENT_RULE(ruleCanBuildRoad)
{
    return true;
}

IMPLEMENT_RULE(ruleSelectRoadway)
{
    if(!player->getIsLocal()) return true;

    Board *board = game->getBoard();
    QList<Roadway*> roadways = board->getRoadways();
    bool selectableObjectFound = false;

    for(int i = 0; i < roadways.size(); i++)
    {
        RULEDATA_PUSH_POINTER("Roadway", roadways.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectRoadway);
        roadways.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        board->setSelectionMode();
        suspendRuleChain();
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleSelectRoadwayAtCrossroad)
{
    RULEDATA_REQUIRE("Crossroad");
    Crossroad *cr = (Crossroad*)RULEDATA_READ_POINTER("Crossroad");
    QList<Roadway*> roadways = cr->getRoadways();
    bool selectableObjectFound = false;
    Board *board = game->getBoard();

    for(int i = 0; i < roadways.size(); i++)
    {
        RULEDATA_PUSH_POINTER("Roadway", roadways.at(i));
        bool selectable = EXECUTE_SUBRULE(ruleCanSelectRoadway);
        roadways.at(i)->setIsSelectable(selectable);
        if(selectable) selectableObjectFound = true;
    }

    if(selectableObjectFound)
    {
        board->setSelectionMode();
        suspendRuleChain();
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleRoadwaySelected)
{
    Board *board = game->getBoard();

    if(board->getHasSelectedObject())
    {
        RULEDATA_PUSH_POINTER("Roadway", board->getSelectedObject());
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleCanSelectRoadway)
{
    RULEDATA_REQUIRE("Roadway");

    Roadway *r = (Roadway*)RULEDATA_POP_POINTER("Roadway");
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

