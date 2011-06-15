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

#include "Utils.h"
#include "FileManager.h"
#include "GraphicsCardSelectionScene.h"
#include "GameCardSelectionDialog.h"

GameCardSelectionDialog::GameCardSelectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(QSize(500,300));
    setModal(true);
    setWindowTitle("Select Cards");
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowTitleHint);

    QGridLayout *l = new QGridLayout();
    l->setVerticalSpacing(20);

    textLabel = new QLabel("Select Cards", this);
    textLabel->setAlignment(Qt::AlignCenter);
    l->addWidget(textLabel, 1, 0);

    graphicsView = new QGraphicsView(this);
    graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);
    graphicsView->viewport()->setAutoFillBackground(false);
    l->addWidget(graphicsView, 0, 0);

    acceptButton = new QPushButton("Accept", this);
    l->addWidget(acceptButton, 2, 0, Qt::AlignCenter);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));

    setLayout(l);

    scene = new GraphicsCardSelectionScene(graphicsView);
    graphicsView->setScene(scene);

    connect(scene, SIGNAL(changed(const QList<QRectF>&)), this,
        SLOT(sceneChanged(const QList<QRectF>&)));

    show(); // required for layout calculation
}

GameCardSelectionDialog::~GameCardSelectionDialog()
{
    delete textLabel;
    delete graphicsView;
    delete acceptButton;
}

const QList<GameCard*>& GameCardSelectionDialog::getSelectedCards()
{
    return scene->getSelectedCards();
}

void GameCardSelectionDialog::sceneChanged(const QList<QRectF> &rects)
{
    Q_UNUSED(rects);

    // update button state
    const QList<GameCard*> cards = scene->getSelectedCards();
    acceptButton->setEnabled(
        (((cards.count() >= numberOfRequiredCards) && !requireExactSelection) ||
        (cards.count() == numberOfRequiredCards)) ||
        (numberOfRequiredCards == 0));
}

void GameCardSelectionDialog::selectCards(GameCardStack *stack,
    bool secret, int amount, bool exact, QString type)
{
    numberOfRequiredCards = amount;
    requireExactSelection = exact;
    acceptButton->setEnabled(false);

    // add cards to scene
    scene->setCards(stack, secret, type);

    // run the dialog
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

