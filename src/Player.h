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

#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QList>
#include <QColor>

#include "GameObject.h"

class Game;
class Resource;
class PlayerObject;

class Player : public GameObject
{
    public:
        Player(Game*);

        const QColor &getColor() { return color; }
        void setColor(QColor c) { color = c; }
        const QString &getName() { return name; }
        void setName(QString n) { name = n; }
        bool getIsLocal() { return isLocal; }
        void setIsLocal(bool b) { isLocal = b; }
        QList<Resource*> &getResources();
        QList<PlayerObject*> &getPlayerObjects();

    private:
        QColor color;
        QString name;
        bool isLocal;
        QList<Resource*> resources;
        QList<PlayerObject*> objects;
};

#endif

