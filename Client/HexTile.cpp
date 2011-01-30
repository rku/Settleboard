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

#include "Game.h"
#include "Crossroad.h"
#include "Roadway.h"
#include "FileManager.h"
#include "HexTile.h"

HexTile::HexTile(Game *_game)
    : GLGameModelProxy(_game)
{
    setIsLightingEnabled(false);
    setType(HEXTILE_TYPE_WATER);
}

HexTile::~HexTile()
{
}

void HexTile::setType(const unsigned int _type)
{
    QColor color;
    QString modelName;

    switch(_type)
    {
        case HEXTILE_TYPE_WATER:
            modelName = "HexTile_Water";
            color.setRgb(22,73,142,255);
            break;
        case HEXTILE_TYPE_DESERT:
            modelName = "HexTile_Desert";
            color.setRgb(225,130,7,255);
            break;
        case HEXTILE_TYPE_WOOD:
            modelName = "HexTile_Wood";
            color.setRgb(27,150,11,255);
            break;
        case HEXTILE_TYPE_SHEEP:
            modelName = "HexTile_Sheep";
            color.setRgb(208,241,206,255);
            break;
        case HEXTILE_TYPE_WEED:
            modelName = "HexTile_Weed";
            color.setRgb(237,239,97,255); 
            break;
        case HEXTILE_TYPE_ORE:
            modelName = "HexTile_Ore";
            color.setRgb(106,112,124,255);
            break;
        case HEXTILE_TYPE_GOLD:
            modelName = "HexTile_Gold"; 
            color.setRgb(222,224,35,255);
            break;
        case HEXTILE_TYPE_CLAY:
            modelName = "HexTile_Clay";
            color.setRgb(231,126,33,255);
            break;
        default:
            qDebug() << "Unknown hextile type:" << _type;
            return;
    }

    type = _type;
    load(FileManager::getPathOfResource("Objects", modelName, "obj"), color);
}

void HexTile::draw()
{
    GLGameModel::draw();
}

unsigned int HexTile::getType()
{
    return type;
}

void HexTile::addCrossroad(Crossroad *crossroad)
{
    Q_ASSERT(crossroads.size() < 6);
    if(!crossroads.contains(crossroad)) crossroads.append(crossroad);
}

void HexTile::addRoadway(Roadway *roadway)
{
    Q_ASSERT(roadways.size() < 6);
    if(!roadways.contains(roadway)) roadways.append(roadway);
}

QVector3D HexTile::getCenterVertex()
{
    return pos;
}

QList<QVector3D> HexTile::getCornerVertices()
{
    QList<QVector3D> vertices;

    vertices = getVerticesOfGroupWithName("CORNERS");
    Q_ASSERT(vertices.size() > 0);

    for(int i = 0; i < vertices.size(); i++)
    {
        vertices[i] += pos;
    }

    return vertices;
}
