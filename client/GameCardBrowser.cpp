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

void GameCardBrowser::init()
{
    setModal(true);

    ui.labelName1->setText("");
    ui.labelName2->setText("");
    ui.labelName3->setText("");
    ui.checkBoxResource->setCheckState(Qt::Unchecked);
    ui.checkBoxDevelopment->setCheckState(Qt::Checked);
    ui.buttonClose->setEnabled(true);
    ui.buttonPlaySelectedCard->setEnabled(false);
    ui.buttonNavigateLeft->setEnabled(false);
    ui.buttonNavigateRight->setEnabled(false);

    position = 0;
}

void GameCardBrowser::update()
{
    GameCardStack *stack = GAME->getLocalPlayer()->getCardStack();
    unsigned int n = stack->getNumberOfCards();
    if(n < 1) return;

    unsigned int middle = qRound(n / 2);
    int startIndex = middle - position - 1;

    if(startIndex < 0) startIndex = 0;
    if(startIndex >= (int)n) startIndex = n - 1;

    // show cards
    QList<GameCard*> cards = stack->getCards();

    ui.labelPixmap1->setPixmap(cards.at(startIndex)->pixmap);
    ui.labelName1->setText(cards.at(startIndex)->name);

    if(cards.size() > (startIndex + 1))
    {
        ui.labelPixmap2->setPixmap(cards.at(startIndex + 1)->pixmap);
        ui.labelName2->setText(cards.at(startIndex + 1)->name);
    }

    if(cards.size() > (startIndex + 2))
    {
        ui.labelPixmap3->setPixmap(cards.at(startIndex + 2)->pixmap);
        ui.labelName3->setText(cards.at(startIndex + 2)->name);
    }
}

void GameCardBrowser::show()
{
    update();
    QDialog::show();
}

