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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <QtGui>

class Game;

class Resource
{
    public:
        // Resource(game,name,initamount=0)
        Resource(Game*, const QString name, const int _initamount = 0);

        void setName(const QString _name) { name = _name; }
        const QString& getName() { return name; }

        unsigned int add(const unsigned int n);
        unsigned int remove(const unsigned int n);
        unsigned int getAmount() { return amount; } 

    private:
        Game *game;
        QString name;
        unsigned int amount;
};

#endif

