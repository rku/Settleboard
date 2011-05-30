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

#include "Player.h"
#include "Game.h"
#include "MessagePanel.h"

MessagePanel::MessagePanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QVBoxLayout *l = new QVBoxLayout();
    QWidget *widget = new QWidget(this);

    input = new QLineEdit(widget);
    input->setAttribute(Qt::WA_MacShowFocusRect, false);
    connect(input, SIGNAL(returnPressed()),
        this, SLOT(newMessageAvailable()));

    output = new QTextEdit(widget);
    output->setReadOnly(true);

    l->addWidget(output);
    l->addWidget(input);

    widget->setLayout(l);
    setWidget(widget);

    setMinimumHeight(200);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));
}

MessagePanel::~MessagePanel()
{
}

void MessagePanel::addSystemMessage(const QString msg)
{
    QString format("! %1");

    output->setTextColor(Qt::white);
    output->append(format.arg(msg));
}

void MessagePanel::addLogMessage(Player *player, const QString msg)
{
    QString format("- %1");

    output->setTextColor(player->getColor().lighter(130));
    output->append(format.arg(msg));
}

void MessagePanel::addChatMessage(Player *player, const QString msg)
{
    QString format("# <%1> %2");

    output->setTextColor(Qt::yellow);
    output->append(format.arg(player->getName()).arg(msg));
}

void MessagePanel::newMessageAvailable()
{
    QString msg = input->text();
    if(!msg.isEmpty())
    {
        input->setText("");
        GAME->getRules()->pushRuleData("ChatMessage", msg);
        GAME->getRules()->executeRule("ruleNewChatMessage");
    }
}

