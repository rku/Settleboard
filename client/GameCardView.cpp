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

#include <QStyle>
#include <QVBoxLayout>
#include <QtDebug>

#include "FileManager.h"
#include "GameCardView.h"

GameCardView::GameCardView(QWidget *parent) : QWidget(parent)
{
    isSelected = false;
    isSecret = false;
    card = NULL;

    QVBoxLayout *l = new QVBoxLayout();

    labelPixmap = new QLabel(this);
    labelPixmap->setScaledContents(false);
    labelPixmap->setObjectName("labelPixmap");
    labelPixmap->setAlignment(Qt::AlignCenter);

    l->addWidget(labelPixmap);

    setLayout(l);

    clear();
}

void GameCardView::clear()
{
    labelPixmap->clear();
    setIsSelected(false);
    card = NULL;
    setVisible(false);
}

void GameCardView::setIsSecret(bool b)
{
    isSecret = b;
    if(card != NULL) setCard(card);
}

void GameCardView::setCard(GameCard *c)
{
    card = c;

    if(card != NULL)
    {
        if(isSecret)
        {
            labelPixmap->setToolTip(QString());
            labelPixmap->setPixmap(GameCard::getCoverPixmap());
        }
        else
        {
            labelPixmap->setToolTip(card->getName());
            labelPixmap->setPixmap(card->getPixmap());
        }

        setVisible(true);
    }
    else clear();
}

void GameCardView::setIsSelected(bool b)
{
    isSelected = false;
    if(!card) return;

    isSelected = b;
    if(b) { emit selected(); }

    style()->unpolish(this);
    style()->polish(this);

    style()->unpolish(labelPixmap);
    style()->polish(labelPixmap);
}

void GameCardView::mousePressEvent(QMouseEvent *event)
{
    if(event->type() & QEvent::MouseButtonPress &&
        event->button() & Qt::LeftButton)
    {
        setIsSelected(true);
    }
}

