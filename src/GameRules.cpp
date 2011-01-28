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
#include "Game.h"

GameRules::GameRules(Game *_game)
    : GameObject(_game)
{
    isRuleChainWaiting = false;

    REGISTER_RULE(ruleUserActionBuildSettlement);
    REGISTER_RULE(ruleBuildSettlement);
    REGISTER_RULE(ruleCanBuildSettlement);
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

        executeRule(rce.name, rce.player);
    }

    if(ruleChain.size() == 0) ruleChainFinished();
}

void GameRules::ruleChainFinished()
{
    qDebug() << "Rule chain finished";
    ruleChain.clear();
    ruleData.clear();
    isRuleChainWaiting = false;
}

void GameRules::cancelCurrentRuleChain()
{
    ruleData.clear();
    ruleChain.clear();
    isRuleChainWaiting = false;

    game->getBoard()->resetBoardState();
}

void GameRules::pushRuleData(void *pointer)
{
    ruleData.push(pointer);
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

void GameRules::handleSelectedObject(GLGameModel*)
{
    Q_ASSERT(ruleChain.size() > 0);
}

// STANDARD RULES

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

    qDebug() << "BUILD SETTLEMENT";
    return true;
}

IMPLEMENT_RULE(ruleCanBuildSettlement)
{
    return true;
}

// Lets the user select a crossroad
IMPLEMENT_RULE(ruleSelectCrossroad)
{
    if(!player->getIsLocal()) return true;

    Board *board = game->getBoard();
    const QList<Crossroad*> crossroads = board->getCrossroads();

    // set all crossroads selectable
    for(int i = 0; i < crossroads.size(); i++)
    {
        ruleData.push(crossroads.at(i));
        bool selectable = EXECUTE_SUBRULE("ruleCanSelectCrossroad");
        crossroads.at(i)->setIsSelectable(selectable);
    }

    board->update();
    suspendRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleCrossroadSelected)
{
    game->getBoard()->resetBoardState();
    return true;
}

// Check if a crossroad can be selected
// it cannot be selected if it has only water tiles around
// since these are the standard rules without seafarers
IMPLEMENT_RULE(ruleCanSelectCrossroad)
{
    if(!player->getIsLocal()) return true;

    Q_ASSERT(ruleData.size() > 0);
    Crossroad *c = (Crossroad*)ruleData.pop();

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
    qDebug() << "Build Road";
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

    for(int i = 0; i < roadways.size(); i++)
    {
        ruleData.push(roadways.at(i));
        bool selectable = EXECUTE_SUBRULE("ruleCanSelectRoadway");
        roadways.at(i)->setIsSelectable(selectable);
    }

    board->update();
    suspendRuleChain();

    return true;
}

IMPLEMENT_RULE(ruleRoadwaySelected)
{
    game->getBoard()->resetBoardState();
    return true;
}

IMPLEMENT_RULE(ruleCanSelectRoadway)
{
    Q_ASSERT(ruleData.size() > 0);
    Roadway *r = (Roadway*)ruleData.pop();

    return true;
}

