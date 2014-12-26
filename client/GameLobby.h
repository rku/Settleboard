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

#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QtWidgets>

#include "ui_gamelobbyform.h"

class GameLobby : public QDialog
{
    Q_OBJECT

    public:
        GameLobby(QWidget *parent = 0);

        void reset();
        void clearPlayerList();
        void update();
        void addChatMessage(const QString &message, const QColor&);

    private slots:
        void chatMessageAvailable();
        void readyStateChanged(int);
        void done(int);

    private:
        void init();
        void setIsPlayerReady(const QString &name, bool);

        QList<QLabel*> nameLabels;
        QList<QLabel*> addressLabels;
        QList<QLabel*> readyLabels;
        QList<QWidget*> colorWidgets;
        uint numberOfPlayers;
        Ui::GameLobbyForm ui;
};

#endif

