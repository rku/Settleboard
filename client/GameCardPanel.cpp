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
#include "GameCard.h"
#include "GameCardPanel.h"

GameCardPanel::GameCardPanel(const QString &title, QWidget *parent)
    : QDockWidget(parent)
{
    // setup ui
    QGridLayout *l = new QGridLayout();
    QWidget *widget = new QWidget(this);

    QLabel *caption = new QLabel("Development Cards:", this);
    l->addWidget(caption, 0, 0, 1, 2);

    cardList = new QListWidget(this);
    l->addWidget(cardList, 1, 0, 1, 2);
    connect(cardList, SIGNAL(itemSelectionChanged()), this,
        SLOT(updateButtonState()));

    buttonInfo = new QPushButton("Info", this);
    buttonInfo->setEnabled(false);
    l->addWidget(buttonInfo, 2, 0);
    connect(buttonInfo, SIGNAL(clicked()), this, SLOT(showCardInfo()));

    buttonPlay = new QPushButton("Play", this);
    buttonPlay->setEnabled(false);
    l->addWidget(buttonPlay, 2, 1);
    connect(buttonPlay, SIGNAL(clicked()), this, SLOT(playCard()));

    l->setColumnStretch(2, 1);
    l->setRowStretch(3, 1);
    widget->setLayout(l);
    setWidget(widget);

    setTitleBarWidget(new QWidget(this));
}

GameCardPanel::~GameCardPanel()
{
}

void GameCardPanel::updateButtonState()
{
    int row = cardList->currentRow();
    bool selected = (cardList->selectedItems().count() > 0);

    // card info button
    buttonInfo->setEnabled(selected);

    // play card button
    bool canPlay = false;
    if(selected && row >= 0)
    {
        GameCard *card = cards.at(row);
        canPlay = GAME->getRules()->executeLocalRule(card->getCanPlayRule());
    }
    buttonPlay->setEnabled(canPlay);
}

void GameCardPanel::showCardInfo()
{
    QDialog *infoDialog = new QDialog(this);

    int row = cardList->currentRow();
    Q_ASSERT(row >= 0);
    GameCard *card = cards.at(row);

    infoDialog->setModal(true);
    infoDialog->setFixedSize(QSize(300, 200));

    QString title("Development Card: %1");
    infoDialog->setWindowTitle(title.arg(card->getName()));

    QGridLayout *layout = new QGridLayout();

    QTextEdit *textView = new QTextEdit(infoDialog);
    textView->setText(card->getDescription());
    textView->setReadOnly(true);
    layout->addWidget(textView, 0, 0);

    QPushButton *buttonHide = new QPushButton("Hide", infoDialog);
    layout->addWidget(buttonHide, 1, 0, Qt::AlignCenter);
    connect(buttonHide, SIGNAL(clicked()), infoDialog, SLOT(accept()));

    infoDialog->setLayout(layout);
    infoDialog->exec();

    delete infoDialog;
}

void GameCardPanel::playCard()
{
    int row = cardList->currentRow();
    Q_ASSERT(row >= 0);

    QString playRule = cards.at(row)->getPlayRule();
    if(!playRule.isEmpty()) GAME->getRules()->executeRule(playRule);
}

void GameCardPanel::clear()
{
    // clear list and cards
    cardList->clear();
    cards.clear();
}

void GameCardPanel::addCard(GameCard *card)
{
    Q_ASSERT(card->getType() == "Development");

    QListWidgetItem *item = new QListWidgetItem(cardList);
    item->setText(card->getName());

    // also save a pointer to the GameCard for each list entry
    // in order to be able to access it easily
    cards.append(card);
}

