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

#define HEXTILE_TYPE_WOOD    0x00 
#define HEXTILE_TYPE_WATER   0x01
#define HEXTILE_TYPE_WEED    0x02
#define HEXTILE_TYPE_SHEEP   0x03
#define HEXTILE_TYPE_DESERT  0x04
#define HEXTILE_TYPE_ORE     0x05
#define HEXTILE_TYPE_CLAY    0x06
#define HEXTILE_TYPE_GOLD    0x07

#define HEXTILE_EDGE_TOP_LEFT       0x00
#define HEXTILE_EDGE_TOP_RIGHT      0x01
#define HEXTILE_EDGE_MIDDLE_LEFT    0x02
#define HEXTILE_EDGE_MIDDLE_RIGHT   0x03
#define HEXTILE_EDGE_BOTTOM_LEFT    0x04
#define HEXTILE_EDGE_BOTTOM_RIGHT   0x05

class HexTile : public GLGameModel
{
    public:
        HexTile(Game*);
        ~HexTile();
        void setType(const unsigned int);
        const QString getType();
        void setFixedPosition(bool b) { fixedPosition = b; }
        bool hasFixedPosition() { return fixedPosition; }
        Vertex3f getEdgeVertex(int name);

    private:
        bool isPort;
        bool fixedPosition;
        int number;
        QString type;
};

#endif /* HEX_TILE_H */

