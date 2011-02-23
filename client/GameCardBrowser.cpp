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

#include "Game.h"
#include "GameCardStack.h"
#include "Player.h"
#include "GameCardBrowser.h"

GameCardBrowser::GameCardBrowser(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setObjectName("gameCardBrowser");
    init();
}

void GameCardBrowser::clear()
{
    ui.labelName1->clear();
    ui.labelPixmap1->clear();
    ui.labelName2->clear();
    ui.labelPixmap2->clear();
    ui.labelName3->clear();
    ui.labelPixmap3->clear();
    ui.labelName4->clear();
    ui.labelPixmap4->clear();

    ui.buttonNavigateLeft->setEnabled(false);
    ui.buttonNavigateRight->setEnabled(false);
}

void GameCardBrowser::init()
{
    setModal(true);

    ui.checkBoxResource->setCheckState(Qt::Unchecked);
    ui.checkBoxDevelopment->setCheckState(Qt::Checked);
    ui.buttonClose->setEnabled(true);
    ui.buttonPlaySelectedCard->setEnabled(false);

    clear();

    connect(ui.buttonNavigateLeft, SIGNAL(clicked()),
        this, SLOT(navigateLeft()));
    connect(ui.buttonNavigateRight, SIGNAL(clicked()),
        this, SLOT(navigateRight()));

    position = 0;
}

void GameCardBrowser::update()
{
    clear();

    GameCardStack *stack = GAME->getLocalPlayer()->getCardStack();
    unsigned int n = stack->getNumberOfCards();
    if(n < 1) return;

    unsigned int middle = qRound(n / 2);
    int startIndex = middle + position - 1;

    if(startIndex < 0) startIndex = 0;
    if(startIndex >= (int)n) startIndex = n - 1;

    // show cards
    QList<GameCard*> cards = stack->getCards();

    int i;
    for(i = 0; i < 4 && cards.size() > i; i++)
    {
        QLabel *lPixmap, *lName;

        switch(i)
        {
            case 0: lPixmap = ui.labelPixmap1; lName = ui.labelName1; break;
            case 1: lPixmap = ui.labelPixmap2; lName = ui.labelName2; break;
            case 2: lPixmap = ui.labelPixmap3; lName = ui.labelName3; break;
            case 3: lPixmap = ui.labelPixmap4; lName = ui.labelName4; break;
            default: Q_ASSERT(false);
        }

        lPixmap->setPixmap(cards.at(startIndex + i)->pixmap);
        lName->setText(cards.at(startIndex + i)->name);
    }

    ui.buttonNavigateRight->setEnabled(cards.size() > (startIndex + i));
    ui.buttonNavigateLeft->setEnabled(startIndex > 0);
}

void GameCardBrowser::show()
{
    position = 0;
    update();
    QDialog::show();
}

void GameCardBrowser::navigateLeft()
{
    position--;
    update();
}

void GameCardBrowser::navigateRight()
{
    position++;
    update();
}

