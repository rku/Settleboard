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

#include "Game.h"
#include "ResourceInfoWidget.h"
#include "GraphicsCardSelectionScene.h"
#include "CreateTradeOfferDialog.h" 

CreateTradeOfferDialog::CreateTradeOfferDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    setFixedSize(QSize(500, 400));

    ui.graphicsViewCardSelection->viewport()->setAutoFillBackground(false);
    ui.widgetResourceInfo->setIsEditable(true);

    // test
    Player *p = GAME->getLocalPlayer();
    GameCardStack *stack = p->getCardStack();
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));
    stack->addCard(new GameCard(GameCard::createResourceCardWheat()));

    cardSelectionScene = new GraphicsCardSelectionScene(this);
    ui.graphicsViewCardSelection->setScene(cardSelectionScene);
}

bool CreateTradeOfferDialog::event(QEvent *event)
{
    bool ret = QDialog::event(event);

    if(event->type() == QEvent::Show)
    {
        GameCardStack *playerStack = GAME->getLocalPlayer()->getCardStack();
        cardSelectionScene->setCards(playerStack, false, "Resource");
    }

    return ret;
}

