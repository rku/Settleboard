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

#include "GraphicsGameCardItem.h"

GraphicsGameCardItem::GraphicsGameCardItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    isSelected = false;
    isSecret = false;
    card = NULL;

    QGraphicsColorizeEffect *colorize = new QGraphicsColorizeEffect();
    colorize->setEnabled(!isSelected);
    colorize->setColor(Qt::black);
    colorize->setStrength(1.0);
    setGraphicsEffect(colorize);
}

void GraphicsGameCardItem::setIsSelected(bool b)
{
    if(b == isSelected) return;

    isSelected = b;
    graphicsEffect()->setEnabled(!isSelected);

    qreal f = 20.0;
    if(isSelected) f *= -1;

    QPointF o = offset();
    o.setY(offset().y() + f);
    setOffset(o);

    update();
}

void GraphicsGameCardItem::setIsSecret(bool b)
{
    if(b == isSecret) return;

    isSecret = b;

    if(card != NULL)
    {
        setPixmap((isSecret) ? GameCard::getCoverPixmap() : card->getPixmap());
        update();
    }
}

void GraphicsGameCardItem::setCard(GameCard *c)
{
    card = c;

    if(card != NULL)
    {
        setPixmap((getIsSecret()) ? GameCard::getCoverPixmap() : c->getPixmap());
        setSelected(false);
    }
}

void GraphicsGameCardItem::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        setIsSelected(!getIsSelected());
    }
}

