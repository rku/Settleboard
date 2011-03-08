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

#ifndef CARDBROWSER_H
#define CARDBROWSER_H 1

#include <QtGui>

#include "GameCard.h"
#include "ui_cardbrowserform.h"

class GameCardStack;

class GameCardBrowser : public QDialog
{
    Q_OBJECT

    public:
        GameCardBrowser(QWidget *parent = 0);

        enum CardBrowserMode { PlayCardMode, SelectCardsMode };
        void show(CardBrowserMode);

        void setCardFilter(const QString &type, const QString &name = QString());
        void setAcceptRule(const QString &s) { acceptRule = s; }
        void setCancelRule(const QString &s) { cancelRule = s; }
        void setCardStack(GameCardStack*);
        void setIsCancelable(bool);
        void setAcceptButtonText(const QString &text);
        void setDescription(const QString &text);
        void setSelectAmount(unsigned int n);

    public slots:
        void close();

    protected slots:
        void clear();
        void update();
        void navigateLeft();
        void navigateRight();
        void cardSelectionChanged();
        void acceptClicked();
        void playSelectedCard();

    private:
        void init();
        void commitCardSelection();

        int position;
        CardBrowserMode mode;
        GameCardStack *cardStack;
        QList<GameCard*> selectedCards;
        bool showCardDescription;
        int selectAmount;
        Ui::CardBrowserForm ui;
        QString cardFilterType;
        QString cardFilterName;
        QString acceptRule;
        QString cancelRule;
};

#endif

