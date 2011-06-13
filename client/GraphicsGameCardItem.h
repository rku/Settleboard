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

#ifndef GRAPHICSGAMECARDITEM_H
#define GRAPHICSGAMECARDITEM_H 1

#include <QtGui>

#include "GameCard.h"

class GraphicsGameCardItem : public QGraphicsPixmapItem
{
    public:
        GraphicsGameCardItem(QGraphicsItem *parent = 0);

        bool getIsSelected() { return isSelected; }
        void setIsSelected(bool b);
        bool getIsSecret() { return isSecret; }
        void setIsSecret(bool b);
        GameCard *getCard() { return card; }
        void setCard(GameCard *c);

    protected:
        void mousePressEvent (QGraphicsSceneMouseEvent *event);

    private:
        bool isSelected;
        bool isSecret;
        GameCard *card;
};

#endif

