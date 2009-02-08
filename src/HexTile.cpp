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

#include "HexTile.h"
#include "Game.h"

#include "OBJGLLoader.h"

HexTile::HexTile(Game *_game) : game(_game)
{
    type = HEX_TILE_TYPE_WATER;
}

HexTile::~HexTile()
{
}

void HexTile::setType(const QString &_type)
{
    type = _type;
}

const QString HexTile::getType()
{
    return type;
}

void HexTile::create()
{
    OBJGLLoader *loader = game->getOBJGLLoader();;

    glNewList(displayListID, GL_COMPILE);

    loader->load("Data/Objects/hextile_water.obj");

    glEndList();
}

