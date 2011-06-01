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
#include "ControlPanel.h"

ControlPanel::ControlPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QGridLayout *l = new QGridLayout();
    QWidget *widget = new QWidget(this);

    buttonCancel = new QToolButton(widget);
    buttonCancel->setVisible(false);
    buttonCancel->setText("Cancel");
    l->addWidget(buttonCancel);
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));

    l->setColumnStretch(5, 1);
    l->setColumnStretch(0, 1);

    widget->setLayout(l);
    setWidget(widget);

    setMaximumHeight(150);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));
}

ControlPanel::~ControlPanel()
{
    clear();
}

void ControlPanel::beginCancelMode()
{
    // hide all buttons
    QList<QToolButton*> b = buttons.values();
    QList<QToolButton*>::iterator i;

    for(i = b.begin(); i != b.end(); ++i)
        (*i)->setVisible(false);

    // show cancel button
    buttonCancel->setVisible(true);
}

void ControlPanel::endCancelMode()
{
    if(!buttonCancel->isVisible()) return;

    // hide cancel button
    buttonCancel->setVisible(false);

    // show alle buttons that are enabled
    QList<QToolButton*> b = buttons.values();
    QList<QToolButton*>::iterator i;

    for(i = b.begin(); i != b.end(); ++i)
        (*i)->setVisible(true);
}

void ControlPanel::cancel()
{
    endCancelMode();
    GAME->getRules()->executeRule("ruleCancel");
}

void ControlPanel::registerAction(const QString name, QAction *action)
{
    Q_ASSERT(!buttons.contains(name));

    QToolButton *button = new QToolButton(this);
    button->setFixedWidth(48);
    button->setFixedHeight(40);
    button->setIconSize(QSize(32,32));
    QGridLayout *l = (QGridLayout*)widget()->layout();

    int row = (l->count() - 1) / 3 + 1;
    int col = (l->count() - 1) % 3 + 1;

    action->setStatusTip(action->toolTip());
    button->setDefaultAction(action);
    l->addWidget(button, row, col);

    l->setRowStretch(row, 0);
    l->setRowStretch(row + 1, 1);

    connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));

    buttons.insert(name, button);
}

void ControlPanel::setActionState(const QString name, bool state)
{
    Q_ASSERT(buttons.contains(name));

    QToolButton *button = buttons.value(name);
    button->setEnabled(state);
    //button->setVisible(state);
}

void ControlPanel::clear()
{
    while(!buttons.isEmpty())
    {
        QToolButton *button = buttons.take(buttons.keys().at(0));

        widget()->layout()->removeWidget(button);
        button->defaultAction()->disconnect(this);
        delete button;
    }
}

void ControlPanel::disable()
{
    // disable all buttons; the have to reenabled by rules now
    QStringList b = buttons.keys();
    QStringList::iterator i = b.begin();
    while(i != b.end()) { setActionState(*i, false); i++; }
    repaint();
}

void ControlPanel::actionTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());

    QString rule = action->data().value<QString>();
    if(rule.isEmpty()) return; // no rule associated with action

    // disable all buttons; the have to reenabled by rules now
    disable();

    qDebug() << "Control panel action triggered. Rule:" << rule;
    GAME->getRules()->executeRule(rule);
}

