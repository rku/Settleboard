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
#include "GraphicsCardSelectionScene.h"
#include "CreateTradeOfferDialog.h" 

CreateTradeOfferDialog::CreateTradeOfferDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    tradeOffer = new TradeOffer(this);

    setFixedSize(QSize(500, 400));

    ui.graphicsViewCardSelection->viewport()->setAutoFillBackground(false);
    ui.widgetWantedResources->setIsEditable(true);

    cardSelectionScene = new GraphicsCardSelectionScene(this);
    ui.graphicsViewCardSelection->setScene(cardSelectionScene);

    connect(ui.buttonPlaceOffer, SIGNAL(clicked()), this, SLOT(placeOffer()));
}

void CreateTradeOfferDialog::placeOffer()
{
    tradeOffer->clear();

    tradeOffer->setFromPlayer(GAME->getLocalPlayer());
    tradeOffer->setIsBankOnly(ui.checkBoxBankOnly->checkState() == Qt::Checked);

    // offered
    const QList<GameCard*> cards = cardSelectionScene->getSelectedCards();
    foreach(GameCard *card, cards)
    {
        tradeOffer->addOfferedResource(card->getName());
    }

    // wanted
    const QMap<QString, int> wantedAmounts =
        ui.widgetWantedResources->getResourceAmounts();
    foreach(QString name, wantedAmounts.keys())
    {
        int amount = wantedAmounts.value(name);
        if(amount < 1) continue;
        tradeOffer->addWantedResource(name, amount);
    }

    // at least one resource has to be wanted or offered
    if( (tradeOffer->getWantedResources().count()  < 1) &&
        (tradeOffer->getOfferedResources().count() < 1))
    {
        QMessageBox::information(this, "No resources selected",
            "You have to select at least one resource to offer or to want!");
        return;
    }

    qDebug() << "Offer created";
    qDebug() << "BankOnly:" << tradeOffer->getIsBankOnly();
    qDebug() << "Offered:" << tradeOffer->getOfferedResources();
    qDebug() << "Wanted:" << tradeOffer->getWantedResources();

    // everything seems to be fine, accept the dialog
    accept();
}

bool CreateTradeOfferDialog::event(QEvent *event)
{
    bool ret = QDialog::event(event);

    if(event->type() == QEvent::Show)
    {
        GameCardStack *playerStack = GAME->getLocalPlayer()->getCardStack();
        cardSelectionScene->setCards(playerStack, false, "Resource");
    }

    return ret;
}

