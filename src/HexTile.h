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

#ifndef HEX_TILE_H
#define HEX_TILE_H 1

#include <QtOpenGL>

#include "GLGameModel.h"

class Game;

#define HEX_TILE_TYPE_WOOD    "wood"
#define HEX_TILE_TYPE_WATER   "water"
#define HEX_TILE_TYPE_WEED    "weed"
#define HEX_TILE_TYPE_SHEEP   "sheep"
#define HEX_TILE_TYPE_DESERT  "desert"
#define HEX_TILE_TYPE_ORE     "ore"
#define HEX_TILE_TYPE_CLAY    "clay"
#define HEX_TILE_TYPE_GOLD    "gold"

class HexTile : public GLGameModel
{
    public:
        HexTile(Game*);
        ~HexTile();
        void setType(const QString&);
        const QString getType();
        void setFixedPosition(bool b) { fixedPosition = b; }
        bool hasFixedPosition() { return fixedPosition; }

    private:
        bool isPort;
        bool fixedPosition;
        int number;
        QString type;
};

#endif /* HEX_TILE_H */

