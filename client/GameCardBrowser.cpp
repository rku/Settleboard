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
    ui.gameCardView1->clear();
    ui.gameCardView2->clear();
    ui.gameCardView3->clear();
    ui.gameCardView4->clear();

    ui.buttonNavigateLeft->setEnabled(false);
    ui.buttonNavigateRight->setEnabled(false);

    position = 0;
    selectedCards.clear();
    cardStack = NULL;
    showCardDescription = true;
    selectAmount = -1;
    cardFilterType.clear();
    cardFilterName.clear();
    acceptRule.clear();
    cancelRule.clear();
}

void GameCardBrowser::init()
{
    setModal(true);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui.buttonClose->setEnabled(true);
    ui.buttonAccept->setEnabled(false);

    clear();

    connect(ui.gameCardView1, SIGNAL(selected()),
        this, SLOT(cardSelectionChanged()));
    connect(ui.gameCardView2, SIGNAL(selected()),
        this, SLOT(cardSelectionChanged()));
    connect(ui.gameCardView3, SIGNAL(selected()),
        this, SLOT(cardSelectionChanged()));
    connect(ui.gameCardView4, SIGNAL(selected()),
        this, SLOT(cardSelectionChanged()));

    connect(ui.buttonNavigateLeft, SIGNAL(clicked()),
        this, SLOT(navigateLeft()));
    connect(ui.buttonNavigateRight, SIGNAL(clicked()),
        this, SLOT(navigateRight()));

    connect(ui.buttonAccept, SIGNAL(clicked()),
        this, SLOT(playSelectedCard()));
}

void GameCardBrowser::update()
{
    Q_ASSERT(cardStack != NULL);

    QList<GameCard*> cards = (cardFilterType.isEmpty()) ?
        cardStack->getCards() : cardStack->getCardsOfType(cardFilterType);
    unsigned int n = cards.size();
    if(n < 1) return;

    unsigned int middle = (n % 2) ? n / 2 : (n - 1) / 2;
    int startIndex = middle + position - 1;

    if(startIndex < 0) startIndex = 0;
    if(startIndex >= (int)n) startIndex = n - 1;

    int i;
    for(i = 0; i < 4 && cards.size() > (startIndex + i); i++)
    {
        GameCardView *view;

        switch(i)
        {
            case 0: view = ui.gameCardView1; break;
            case 1: view = ui.gameCardView2; break;
            case 2: view = ui.gameCardView3; break;
            case 3: view = ui.gameCardView4; break;
            default: Q_ASSERT(false);
        }

        view->setCard(cards.at(startIndex + i));
    }

    ui.buttonNavigateRight->setEnabled(cards.size() > (startIndex + i));
    ui.buttonNavigateLeft->setEnabled(startIndex > 0);
}

void GameCardBrowser::show(CardBrowserMode m)
{
    mode = m;
    position = 0;
    update();
    if(selectAmount < 1) ui.gameCardView1->setIsSelected(true);
    resize(QSize(1,1));
    adjustSize();
    QDialog::show();
}

void GameCardBrowser::close()
{
    clear();
    QDialog::close();
}

void GameCardBrowser::setCardFilter(const QString &type, const QString &name)
{
    cardFilterType = type;
    cardFilterName = name;
    update();
}

void GameCardBrowser::setCardStack(GameCardStack *stack)
{
    cardStack = stack;
    if(isVisible()) update();
}

void GameCardBrowser::setIsCancelable(bool b)
{
    ui.buttonClose->setVisible(b);
    if(isVisible()) update();
}

void GameCardBrowser::setAcceptButtonText(const QString &text)
{
    ui.buttonAccept->setText(text);
    if(isVisible()) update();
}

void GameCardBrowser::setDescription(const QString &text)
{
    ui.textEditDescription->setText(text);
    if(isVisible()) update();
}

void GameCardBrowser::setSelectAmount(unsigned int n)
{
    selectAmount = n;
    if(isVisible()) update();
}

void GameCardBrowser::navigateLeft()
{
    position--;
    if(isVisible()) update();
}

void GameCardBrowser::navigateRight()
{
    position++;
    if(isVisible()) update();
}

void GameCardBrowser::cardSelectionChanged()
{
    GameCardView *l = qobject_cast<GameCardView*>(sender());
    Q_ASSERT(l);

    if(l != ui.gameCardView1) ui.gameCardView1->setIsSelected(false);
    if(l != ui.gameCardView2) ui.gameCardView2->setIsSelected(false);
    if(l != ui.gameCardView3) ui.gameCardView3->setIsSelected(false);
    if(l != ui.gameCardView4) ui.gameCardView4->setIsSelected(false);

    GameCard *card = l->getCard();
    ui.textEditDescription->setText(QString("%1:\n\n%2")
        .arg(card->getName()).arg(card->getDescription()));

    if(selectAmount < 1)
    {
        // single selection
        selectedCards.clear();
        selectedCards.append(card);
    }
    else
    {
        if(selectedCards.contains(card))
        {
            selectedCards.removeAll(card);
        }
        else
        {
            selectedCards.append(card);
        }
    }

    bool canPlayCard = GAME->getRules()->executeLocalRule(card->getCanPlayRule());
    ui.buttonAccept->setEnabled(canPlayCard);
    update();
}

void GameCardBrowser::acceptClicked()
{
    switch(mode)
    {
        case PlayCardMode:
            playSelectedCard();
            break;
        case SelectCardsMode:
            commitCardSelection();
            break;
        default:
            Q_ASSERT(false);
    }
}

void GameCardBrowser::playSelectedCard()
{
    Q_ASSERT(selectedCards.size() == 1);
    close();

    QString playRule = selectedCards.at(0)->getPlayRule();
    if(!playRule.isEmpty()) GAME->getRules()->executeRule(playRule);
}

void GameCardBrowser::commitCardSelection()
{
    Q_ASSERT(!acceptRule.isEmpty());

    GAME->getRules()->executeRule(acceptRule);
}

