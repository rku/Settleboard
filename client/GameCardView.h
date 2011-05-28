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

#ifndef GAMECARDVIEW_H
#define GAMECARDVIEW_H 1

#include <QLabel>
#include <QMouseEvent>

#include "GameCard.h"

class GameCardView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isSelected READ getIsSelected WRITE setIsSelected);

    public:
        GameCardView(QWidget *parent = 0);

        void clear();

        void setCard(GameCard*);
        GameCard* getCard() { return card; }

        void setIsSelected(bool);
        bool getIsSelected() { return isSelected; }
        void setIsSecret(bool);
        bool getIsSecret() { return isSecret; }

    signals:
        void selected();

    protected:
        void mousePressEvent(QMouseEvent*);

        QLabel *labelPixmap;
        bool isSelected;
        bool isSecret;
        GameCard *card;
};

#endif

