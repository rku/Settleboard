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

#include "ui_cardbrowserform.h"

class GameCardBrowser : public QDialog
{
    Q_OBJECT

    public:
        GameCardBrowser(QWidget *parent = 0);

        void show();

    protected slots:
        void clear();
        void update();
        void navigateLeft();
        void navigateRight();
        void cardSelectionChanged();

    private:
        void init();

        int position;
        Ui::CardBrowserForm ui;
};

#endif

