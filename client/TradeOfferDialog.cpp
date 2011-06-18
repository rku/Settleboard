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

#include "Game.h"
#include "ResourceInfoWidget.h"
#include "TradeOfferDialog.h" 

TradeOfferDialog::TradeOfferDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.buttonAccept, SIGNAL(clicked()), this, SLOT(acceptOffer()));
    connect(ui.buttonReject, SIGNAL(clicked()), this, SLOT(rejectOffer()));
    connect(ui.buttonCounterOffer, SIGNAL(clicked()), this, SLOT(counterOffer()));
}

void TradeOfferDialog::acceptOffer()
{
    GameRules *rules = GAME->getRules();

    rules->pushRuleData(tradeOffer);
    rules->executeRule("ruleAcceptTradeOffer");
}

void TradeOfferDialog::rejectOffer()
{
    GameRules *rules = GAME->getRules();

    rules->pushRuleData(tradeOffer);
    rules->executeRule("ruleRejectTradeOffer");
}

void TradeOfferDialog::counterOffer()
{
    GAME->getRules()->executeRule("ruleCreateCounterOffer");
}

void TradeOfferDialog::setTradeOffer(TradeOffer *offer)
{
    Q_ASSERT(offer != NULL);
    tradeOffer = offer;

    Player *fromPlayer = tradeOffer->getFromPlayer();
    Player *toPlayer = tradeOffer->getToPlayer();

    ui.labelFromPlayer->setText(
        (fromPlayer == NULL) ? "Unknown" : fromPlayer->getName());
    ui.labelToPlayer->setText(
        (toPlayer == NULL) ? "Everyone" : toPlayer->getName());

    // set offered resources
    ResourceInfoWidget *offered = ui.widgetResourcesOffered;
    QMap<QString, int> resourcesOffered = offer->getOfferedResources();
    foreach(QString name, resourcesOffered.keys())
    {
        offered->setResourceAmount(name, resourcesOffered.value(name));
    }

    // set wanted resources
    ResourceInfoWidget *wanted = ui.widgetResourcesWanted;
    QMap<QString, int> resourcesWanted = offer->getWantedResources();
    foreach(QString name, resourcesWanted.keys())
    {
        wanted->setResourceAmount(name, resourcesWanted.value(name));
    }

    // update button states
    ui.buttonAccept->setEnabled(fromPlayer != GAME->getLocalPlayer());
    ui.buttonReject->setEnabled(ui.buttonAccept->isEnabled());
    ui.buttonCounterOffer->setEnabled(ui.buttonAccept->isEnabled());
}

