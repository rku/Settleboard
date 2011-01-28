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
#include "HexTile.h"

HexTile::HexTile(Game *_game)
    : GLGameModel(_game)
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
    QString model("Data/Objects/%1.obj");
    QString modelName;

    switch(_type)
    {
        case HEXTILE_TYPE_WATER:
            modelName = "hextile_water";
            color.setRgb(22,73,142,255);
            break;
        case HEXTILE_TYPE_DESERT:
            modelName = "hextile_desert";
            color.setRgb(225,130,7,255);
            break;
        case HEXTILE_TYPE_WOOD:
            modelName = "hextile_wood";
            color.setRgb(27,150,11,255);
            break;
        case HEXTILE_TYPE_SHEEP:
            modelName = "hextile_sheep";
            color.setRgb(208,241,206,255);
            break;
        case HEXTILE_TYPE_WEED:
            modelName = "hextile_weed";
            color.setRgb(237,239,97,255); 
            break;
        case HEXTILE_TYPE_ORE:
            modelName = "hextile_ore";
            color.setRgb(106,112,124,255);
            break;
        case HEXTILE_TYPE_GOLD:
            modelName = "hextile_gold"; 
            color.setRgb(222,224,35,255);
            break;
        case HEXTILE_TYPE_CLAY:
            modelName = "hextile_clay";
            color.setRgb(231,126,33,255);
            break;
        default:
            qDebug() << "Unknown hextile type:" << _type;
            return;
    }

    type = _type;
    load(model.arg(modelName), color);
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

Vertex3f HexTile::getCenterVertex()
{
    Vertex3f c;
    c.x = posX; c.y = posY; c.z = posZ;
    return c;
}

QList<Vertex3f> HexTile::getCornerVertices()
{
    QList<Vertex3f> vertices;

    vertices = getVerticesOfGroupWithName("CORNERS");
    Q_ASSERT(vertices.size() > 0);

    for(int i = 0; i < vertices.size(); i++)
    {
        vertices[i].x += posX;
        vertices[i].y += posY;
        vertices[i].z += posZ;
    }

    return vertices;
}

