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

#ifndef TRADEOFFERDIALOG_H
#define TRADEOFFERDIALOG_H 1

#include <QtGui>

#include "TradeOffer.h"

#include "ui_tradeofferform.h"

class TradeOfferDialog : public QDialog
{
    Q_OBJECT

    public:
        TradeOfferDialog(QWidget *parent = 0);

        void setTradeOffer(TradeOffer *offer);
        const TradeOffer* getTradeOffer() { return tradeOffer; }

    protected slots:
        void acceptOffer();
        void rejectOffer();
        void counterOffer();

    private:
        Ui::TradeOfferForm ui;
        TradeOffer *tradeOffer;
};

#endif

