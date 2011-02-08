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

#include "GameLobby.h" 

GameLobby::GameLobby(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    setModal(true);
    ui.listWidget->setSelectionMode(QAbstractItemView::NoSelection);
}

void GameLobby::clearPlayerList()
{
    ui.listWidget->clear();
}

void GameLobby::addPlayer(const QString &name, const QString address,
    const QColor &color)
{
    Q_UNUSED(address);

    QListWidgetItem *item = new QListWidgetItem(name, ui.listWidget);
    item->setForeground(QBrush(color));
}

