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

#include <QtDebug>

#include "Map.h"

Map::Map(QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing map...";
}

Map::~Map()
{
    while(!tiles.isEmpty()) delete tiles.takeFirst();
}

void Map::createTile(unsigned int x, unsigned int y,
    HexTile::HexTileType type, bool isFixed, int chipNumber,
    HexTile::HexTileType portType)
{
    Q_ASSERT(size.width() > (int)x);
    Q_ASSERT(size.height() > (int)y);

    HexTile *tile = new HexTile(type, x, y, this);
    tile->setHasFixedPosition(isFixed);
    //tile->setHasPort(hasPort);

    if(chipNumber > 1 && chipNumber < 13 && chipNumber != 7)
        tile->setChipNumber(chipNumber);

    tiles.append(tile);
}

