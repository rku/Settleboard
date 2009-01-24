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

    texture = game->getTextureManager()->getTexture(_type);
}

const QString HexTile::getType()
{
    return type;
}

void HexTile::create()
{
    GLfloat objectData[] = {
        -1.25f,  0.00f,  0.0f,
        -0.50f,  0.00f, -1.0f,
         0.50f,  0.00f, -1.0f,
         1.25f,  0.00f,  0.0f,
         0.50f,  0.00f,  1.0f,
        -0.50f,  0.00f,  1.0f,

        -1.25f, -0.10f,  0.0f,
        -0.50f, -0.10f, -1.0f,
         0.50f, -0.10f, -1.0f,
         1.25f, -0.10f,  0.0f,
         0.50f, -0.10f,  1.0f,
        -0.50f, -0.10f,  1.0f
    },
    texData[] = {
        0.00f, 0.5f,
        0.25f, 1.0f,
        0.75f, 1.0f,
        1.00f, 0.5f,
        0.75f, 0.0f,
        0.25f, 0.0f,

        0.00f, 0.5f,
        0.25f, 1.0f,
        0.75f, 1.0f,
        1.00f, 0.5f,
        0.75f, 0.0f,
        0.25f, 0.0f 
    };
    GLubyte hexTileIndices[] = {
        0,  5,  4,  3,  2,  1, 0, 
        6,  7,  8,  9, 10, 11, 6, 
        0,  1,  7,  6, 
        1,  2,  8,  7, 
        2,  3,  9,  8, 
        3,  4, 10,  9, 
        4,  5, 11, 10,
        5,  0,  6, 11 
    };

    displayListID = glGenLists(1);

    glNewList(displayListID, GL_COMPILE);

    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texData);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, objectData);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glDrawElements(GL_POLYGON, 7, GL_UNSIGNED_BYTE, &hexTileIndices);
    glDrawElements(GL_POLYGON, 7, GL_UNSIGNED_BYTE, &hexTileIndices[ 7]);

    glDisable(GL_TEXTURE_COORD_ARRAY);

    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &hexTileIndices[14]);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &hexTileIndices[18]);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &hexTileIndices[22]);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &hexTileIndices[26]);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &hexTileIndices[30]);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &hexTileIndices[34]);

    glEndList();
}

