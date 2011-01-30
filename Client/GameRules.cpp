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
#include "Game.h"

GameRules::GameRules(Game *_game)
    : GameObject(_game)
{
    isRuleChainWaiting = false;

    REGISTER_RULE(ruleInitGameCards);

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
        ruleChain.append(rce);
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
        RuleChainElement rce = ruleChain.pop();
        isRuleChainWaiting = rce.suspend;
        if(rce.suspend) break;

        // cancel rule, if a rule fails
        if(!executeRule(rce.name, rce.player)) cancelRuleChain();
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

IMPLEMENT_RULE(ruleInitGameCards)
{
    Bank *bank = game->getBank();

    // resources
    bank->registerCardStack("Wheat");
    bank->registerCardStack("Sheep");
    bank->registerCardStack("Ore");
    bank->registerCardStack("Lumber");

    bank->registerCard("Wheat",  GameCard("Resource", "Wheat",  "Wheat" ), 20);
    bank->registerCard("Sheep",  GameCard("Resource", "Sheep",  "Sheep" ), 20);
    bank->registerCard("Ore",    GameCard("Resource", "Ore",    "Ore"   ), 20);
    bank->registerCard("Lumber", GameCard("Resource", "Lumber", "Lumber"), 20);

    // development cards
    bank->registerCardStack("Development");
    bank->registerCard("Development", GameCard("Development", "Knight",
        "If you play this card, you can steal a card from another player.",
        "rulePlayKnightCard"), 10);
    bank->registerCard("Development", GameCard("Development", "Build Road",
        "Playing this card will allow you to place two roads for free.",
        "rulePlayBuildRoadCard"), 4);

    return true;
}

IMPLEMENT_RULE(ruleUserActionBuildCity)
{
    if(player->getIsLocal())
    {
        RULECHAIN_ADD("ruleBuildCity");
        RULECHAIN_ADD("ruleSettlementSelected");
        RULECHAIN_ADD("ruleSelectSettlement");
        startRuleChain();
    }

    return true;
}

IMPLEMENT_RULE(ruleBuildCity)
{
    if(!EXECUTE_SUBRULE("ruleCanBuildCity"))
    {
        qDebug() << "Cannot build city!";
        return false;
    }

    Q_ASSERT(ruleData.contains("Crossroad"));
    Crossroad *cr = (Crossroad*)ruleData.take("Crossroad");

    // remove settlement
    Q_ASSERT(cr->getIsPlayerObjectPlaced());
    ruleData.insert("Settlement", cr->getPlayerObject());
    if(EXECUTE_SUBRULE("ruleRemoveSettlement"))
    {
        // build city
        PlayerObject *bld = new PlayerObject(game, player, "City");
        bld->setScale(0.7);
        cr->placePlayerObject(bld);
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
        ruleData.insert("Crossroad", board->getSelectedObject());
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleUserActionBuildSettlement)
{
    if(player->getIsLocal())
    {
        RULECHAIN_ADD("ruleBuildSettlement");
        RULECHAIN_ADD("ruleCrossroadSelected");
        RULECHAIN_ADD("ruleSelectCrossroad");
        startRuleChain();
    }

    return true;
}

IMPLEMENT_RULE(ruleBuildSettlement)
{
    if(!EXECUTE_SUBRULE("ruleCanBuildSettlement"))
    {
        qDebug() << "Cannot build settlement!";
        return false;
    }

    Q_ASSERT(ruleData.contains("Crossroad"));
    Crossroad *cr = (Crossroad*)ruleData.take("Crossroad");

    Q_ASSERT(!cr->getIsPlayerObjectPlaced());
    PlayerObject *bld = new PlayerObject(game, player, "Settlement");
    bld->setScale(0.3);
    cr->placePlayerObject(bld);

    return true;
}

IMPLEMENT_RULE(ruleCanBuildSettlement)
{
    return true;
}

IMPLEMENT_RULE(ruleRemoveSettlement)
{
    Q_ASSERT(ruleData.contains("Settlement"));
    PlayerObject *b = (PlayerObject*)ruleData.take("Settlement");
    delete b;

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
        ruleData.insert("Crossroad", crossroads.at(i));
        bool selectable = EXECUTE_SUBRULE("ruleCanSelectCrossroad");
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

    if(board->getHasSelectedObject())
    {
        ruleData.insert("Crossroad", board->getSelectedObject());
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

    Q_ASSERT(ruleData.contains("Crossroad"));
    Crossroad *c = (Crossroad*)ruleData.take("Crossroad");

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
        RULECHAIN_ADD("ruleBuildRoad");
        RULECHAIN_ADD("ruleRoadwaySelected");
        RULECHAIN_ADD("ruleSelectRoadway");
        startRuleChain();
    }

    return true;
}

IMPLEMENT_RULE(ruleBuildRoad)
{
    if(!EXECUTE_SUBRULE("ruleCanBuildRoad"))
    {
        qDebug() << "Cannot build road!";
        return false;
    }

    Q_ASSERT(ruleData.contains("Roadway"));
    Roadway *r = (Roadway*)ruleData.take("Roadway");
    PlayerObject *road = new PlayerObject(game, player, "Road");
    road->setScale(0.3);
    r->placePlayerObject(road);
    
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
        ruleData.insert("Roadway", roadways.at(i));
        bool selectable = EXECUTE_SUBRULE("ruleCanSelectRoadway");
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
        ruleData.insert("Roadway", board->getSelectedObject());
        return true;
    }

    return false;
}

IMPLEMENT_RULE(ruleCanSelectRoadway)
{
    Q_ASSERT(ruleData.contains("Roadway"));
    Roadway *r = (Roadway*)ruleData.take("Roadway");
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

