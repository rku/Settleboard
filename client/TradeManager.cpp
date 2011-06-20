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

#include <QtDebug>

#include "Game.h"
#include "TradeDialog.h"
#include "TradeManager.h"

TradeManager::TradeManager(QObject *parent) : QObject(parent)
{
    tradeDialog = new TradeDialog();
    tradeDialog->setModal(true);
}

TradeManager::~TradeManager()
{
    delete tradeDialog;
}

void TradeManager::showDialog()
{
    tradeDialog->show();
}

void TradeManager::placeOffer(const TradeOffer *offer)
{
    GAME->getRules()->pushRuleData(const_cast<TradeOffer*>(offer));
    GAME->getRules()->executeRule("rulePlaceTradeOffer");
}

void TradeManager::addOffer(TradeOffer *offer)
{
    trades.insert(offer->getFromPlayer(), offer);
}

void TradeManager::clear()
{
    foreach(Player *p, trades.keys()) delete trades.take(p);
}

