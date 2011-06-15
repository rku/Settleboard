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

#include "FileManager.h"
#include "GraphicsGameCardItem.h"
#include "Utils.h"
#include "GraphicsCardSelectionScene.h"

GraphicsCardSelectionScene::GraphicsCardSelectionScene(QWidget *parent)
    : QGraphicsScene(parent)
{
}

GraphicsCardSelectionScene::~GraphicsCardSelectionScene()
{
}

void GraphicsCardSelectionScene::calculateScene()
{
    // check if there is a view this scene is drawn in
    if(views().isEmpty()) return;
    QGraphicsView *view = views().first();
    
    int n = items().count();
    if(n < 1) return; 

    qreal w, h, xoffset;
    qreal f = 1.0;

    do {
        w = view->viewport()->width() * f * 0.8; // the width of the arc
        h = view->viewport()->height() * f * 0.6; // the height of the arc
        xoffset = w / n;
        f -= 0.1;
    } while(xoffset >= 45.0 && f > 0.1);

    qreal x = - xoffset * ((n == 1) ? 0 : (n / 2)); // starting position
    qreal r = (w*w + h*h) / (2 * h); // the radius of the circle as a function of w and h

    if(n % 2) x -= xoffset / 2;

    for(int i = 0; i < n; ++i)
    {
        GraphicsGameCardItem *item;
        item = static_cast<GraphicsGameCardItem*>(items().at(i));

        qreal y = r + qSqrt(r*r - x*x); // the y offset for this card on the arc

        item->setOffset(x, -y);
        item->setTransformOriginPoint(item->offset());
        item->setRotation(360 * qAsin(x/r) / (2 * M_PI)); // the card's rotation angle as degrees
        x += xoffset;
    }

    QRectF rect = sceneRect();
    setSceneRect(rect.x(), rect.y() -50.0, rect.width(), rect.height() +50.0);
}

void GraphicsCardSelectionScene::updateSelectedCards()
{
    selectedCards.clear();

    for(int i = 0; i < items().count(); ++i)
    {
        GraphicsGameCardItem *item;
        item = static_cast<GraphicsGameCardItem*>(items().at(i));

        if(item->getIsSelected())
        {
            selectedCards.append(item->getCard());
        }
    }
}

const QList<GameCard*>& GraphicsCardSelectionScene::getSelectedCards()
{
    updateSelectedCards();
    return selectedCards;
}

void GraphicsCardSelectionScene::setCards(GameCardStack *stack,
    bool secret, QString type)
{
    // add cards to scene
    for(unsigned int i = 0; i < stack->getNumberOfCards(); ++i)
    {
        GameCard *card = stack->getCards().at(i);
        if(!type.isNull() && card->getType() != type) continue;

        GraphicsGameCardItem *item = new GraphicsGameCardItem();
        item->setCard(card);
        item->setIsSecret(secret);
        addItem(item);
    }

    calculateScene();
    update();
}

