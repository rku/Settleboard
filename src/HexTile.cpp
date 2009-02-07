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

    texture = game->getTextureManager()->getTexture(_type);
}

const QString HexTile::getType()
{
    return type;
}

void HexTile::create()
{
    OBJGLLoader *loader = new OBJGLLoader();

    displayListID = glGenLists(1);

    glNewList(displayListID, GL_COMPILE);

    loader->load("Data/Objects/deserttile.obj");
    delete loader;

    /*glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texData);
    glEnableClientState(GL_VERTEX_ARRAY);
    // glVertexPointer(3, GL_FLOAT, 0, objectData);
    glVertexPointer(3, GL_FLOAT, 0, vdata);
    
    glBegin(GL_POLYGON);

    for(int i = 0; i < fdata.size(); ++i)
    {
        int pos = (fdata[i] - 1) * 3;
        glVertex3f(odata.at(pos), odata.at(pos+1), odata.at(pos+2));
    }
    glEnd();

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
    */

    glEndList();
}

