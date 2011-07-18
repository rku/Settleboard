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
#include "FileManager.h"
#include "TradeManager.h"
#include "CreateTradeOfferDialog.h"
#include "TradeOffer.h"
#include "TradeDialog.h" 

TradeDialog::TradeDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    ui.listWidgetTradeOffers->viewport()->setAutoFillBackground(false);

    connect(ui.buttonCreateTrade, SIGNAL(clicked()),
        this, SLOT(createTradeOffer()));
    connect(ui.buttonShowSelectedOffer, SIGNAL(clicked()),
        this, SLOT(showSelectedOffer()));
    connect(ui.listWidgetTradeOffers, SIGNAL(currentRowChanged(int)),
        this, SLOT(selectionChanged(int)));

    setFixedSize(QSize(500,220));
}

void TradeDialog::createTradeOffer()
{
    CreateTradeOfferDialog *dlg = new CreateTradeOfferDialog(this);
    
    if(dlg->exec() == QDialog::Accepted)
    {
        TradeOffer *offer = dlg->getTradeOffer();
        GAME->getTradeManager()->placeOffer(offer);
    }

    delete dlg;
}

void TradeDialog::selectionChanged(int row)
{
    QListWidgetItem *current = ui.listWidgetTradeOffers->currentItem();
    ui.buttonShowSelectedOffer->setEnabled(current != NULL);
}

void TradeDialog::showSelectedOffer()
{
    QListWidgetItem *current = ui.listWidgetTradeOffers->currentItem();
    Q_ASSERT(trades.contains(current));

    if(current != NULL)
    {
        TradeOffer *offer = trades.value(current);
        offer->show();
    }
}

void TradeDialog::addTrade(TradeOffer *offer)
{
    Q_ASSERT(!trades.values().contains(offer));

    // text and icon
    QString tmpl;
    QString iconName;

    switch(offer->getState())
    {
        case TradeOffer::OfferAccepted:
            iconName = "TradeOk.png";
            tmpl = "%1 accepted your offer";
            break;
        case TradeOffer::OfferRejected:
            iconName = "TradeReject.png";
            tmpl = "%1 rejected your offer";
            break;
        default:
            iconName = "TradeAsk.png";
            tmpl = "%1 created a counter offer";
            break;
    }

    QString name = (offer->getFromPlayer() == NULL) ?
        "The bank" : offer->getFromPlayer()->getName();
    QString text = QString(tmpl).arg(name);
    QString iconFilename = FileManager::getPathOfImage(iconName);
    QIcon icon = QIcon(iconFilename);
    QListWidget *parent = ui.listWidgetTradeOffers;

    QListWidgetItem *item = new QListWidgetItem(icon, text, parent);
    trades.insert(item, offer);
    parent->update();
}

void TradeDialog::removeTrade(TradeOffer *offer)
{
    foreach(QListWidgetItem *item, trades.keys())
    {
        if(trades.value(item) == offer)
        {
            trades.remove(item);
            delete item;
        }
    }
}

void TradeDialog::clear()
{
    foreach(QListWidgetItem *item, trades.keys())
    {
        trades.remove(item);
        delete item;
    }

    update();
}

