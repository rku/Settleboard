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
#include "GameCardSelectionDialog.h"

GameCardSelectionDialog::GameCardSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *l = new QGridLayout();
    l->setVerticalSpacing(20);

    textLabel = new QLabel("Select Cards", this);
    textLabel->setAlignment(Qt::AlignCenter);
    l->addWidget(textLabel, 1, 0);

    graphicsView = new QGraphicsView(this);
    l->addWidget(graphicsView, 0, 0);

    acceptButton = new QPushButton("Accept", this);
    l->addWidget(acceptButton, 2, 0, Qt::AlignCenter);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));

    setLayout(l);

    scene = new QGraphicsScene(graphicsView);
    graphicsView->setScene(scene);
    graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    graphicsView->viewport()->setAutoFillBackground(false);

    connect(scene, SIGNAL(changed(const QList<QRectF>&)), this,
        SLOT(sceneChanged(const QList<QRectF>&)));

    setFixedSize(QSize(500,300));
    setModal(true);
    setWindowTitle("Select Cards");
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowTitleHint);
}

GameCardSelectionDialog::~GameCardSelectionDialog()
{
    delete textLabel;
    delete graphicsView;
    delete acceptButton;
}

void GameCardSelectionDialog::sceneChanged(const QList<QRectF> &rects)
{
    Q_UNUSED(rects);

    // update button state
    updateSelectedCards();
    acceptButton->setEnabled(
        (((selectedCards.count() >= numberOfRequiredCards) && !requireExactSelection) ||
        (selectedCards.count() == numberOfRequiredCards)) ||
        (numberOfRequiredCards == 0));
}

void GameCardSelectionDialog::drawScene()
{
    QList<QGraphicsItem*> items = scene->items();
    int n = items.count();
    if(n < 1) return; 

    qreal w, h, xoffset;
    qreal f = 1.0;

    do {
        w = width() * f * 0.8; // the width of the arc
        h = height() * f * 0.6; // the height of the arc
        xoffset = w / n;
        f -= 0.1;
    } while(xoffset >= 45.0 && f > 0.1);

    qreal x = - xoffset * ((n == 1) ? 0 : (n / 2)); // starting position
    qreal r = (w*w + h*h) / (2 * h); // the radius of the circle as a function of w and h

    if(n % 2) x -= xoffset / 2;

    for(int i = 0; i < n; ++i)
    {
        GraphicsGameCardItem *item;
        item = static_cast<GraphicsGameCardItem*>(items.at(i));

        qreal y = r + qSqrt(r*r - x*x); // the y offset for this card on the arc

        item->setOffset(x, -y);
        item->setTransformOriginPoint(item->offset());
        item->setRotation(360 * qAsin(x/r) / (2 * M_PI)); // the card's rotation angle as degrees
        x += xoffset;
    }

    QRectF rect = scene->sceneRect();
    scene->setSceneRect(rect.x(), rect.y() - 50.0,
        rect.width(), rect.height() + 50.0);
}

void GameCardSelectionDialog::updateSelectedCards()
{
    selectedCards.clear();

    for(int i = 0; i < scene->items().count(); ++i)
    {
        GraphicsGameCardItem *item;
        item = static_cast<GraphicsGameCardItem*>(scene->items().at(i));

        if(item->getIsSelected())
        {
            selectedCards.append(item->getCard());
        }
    }
}

void GameCardSelectionDialog::selectCards(GameCardStack *stack,
    bool secret, int amount, bool exact, QString type)
{
    numberOfRequiredCards = amount;
    requireExactSelection = exact;
    acceptButton->setEnabled(false);

    // add cards to scene
    for(unsigned int i = 0; i < stack->getNumberOfCards(); ++i)
    {
        GameCard *card = stack->getCards().at(i);
        if(!type.isNull() && card->getType() != type) continue;

        GraphicsGameCardItem *item = new GraphicsGameCardItem();
        item->setCard(card);
        item->setIsSecret(secret);
        scene->addItem(item);
    }

    drawScene();
    exec();
}

void GameCardSelectionDialog::setAcceptActionText(QString text)
{
    acceptButton->setText(text);
}

void GameCardSelectionDialog::setDescriptionText(QString text)
{
    textLabel->setText(text);
}

