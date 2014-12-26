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
    if(tradeOffer->getState() == TradeOffer::OfferAccepted)
    {
        // execute trade
        tradeOffer->execute();
        return;
    }

    tradeOffer->accept();
}

void TradeOfferDialog::rejectOffer()
{
    tradeOffer->reject();
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
    TradeOffer::TradeOfferState state = tradeOffer->getState();

    ui.labelFromPlayer->setText(
        (tradeOffer->getFromPlayer() == NULL) ?
            QString("Bank") : fromPlayer->getName());
    ui.labelToPlayer->setText(
        (tradeOffer->getToPlayer() == NULL) ?
            QString("Bank") : toPlayer->getName());

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
    GAME->getRules()->pushRuleData("TradeOfferId", tradeOffer->getId());
    ui.buttonAccept->setEnabled(fromPlayer != GAME->getLocalPlayer()
        && GAME->getRules()->executeLocalRule("ruleCanExecuteTrade"));
    ui.buttonReject->setEnabled(fromPlayer != GAME->getLocalPlayer());
    ui.buttonCounterOffer->setEnabled(ui.buttonReject->isEnabled());

    // button texts and visiblity
    ui.buttonAccept->setText((state == TradeOffer::OfferPlaced) ?
        "Accept" : "Trade");
    ui.buttonAccept->setVisible((state != TradeOffer::OfferRejected));
    ui.buttonReject->setVisible((state == TradeOffer::OfferPlaced));
    ui.buttonCounterOffer->setVisible(ui.buttonReject->isVisible());
}

