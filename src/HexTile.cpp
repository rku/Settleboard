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

HexTile::HexTile(Game *_game)
    : GLGameModel(_game)
{
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

const QString HexTile::getType()
{
    return type;
}

Vertex3f HexTile::getEdgeVertex(int which)
{
    QString name;
    QList<Vertex3f> vertices;

    switch(which)
    {
        case HEXTILE_EDGE_TOP_LEFT: name = "CROSS_TL"; break;
        case HEXTILE_EDGE_TOP_RIGHT: name = "CROSS_TR"; break;
        case HEXTILE_EDGE_MIDDLE_LEFT: name = "CROSS_ML"; break;
        case HEXTILE_EDGE_MIDDLE_RIGHT: name = "CROSS_RL"; break;
        case HEXTILE_EDGE_BOTTOM_LEFT: name = "CROSS_BL"; break;
        case HEXTILE_EDGE_BOTTOM_RIGHT: name = "CROSS_BR"; break;
        default: Q_ASSERT(false);
    }

    vertices = getVerticesOfGroupWithName(name);
    Q_ASSERT(vertices.size() > 0);

    return vertices.at(0);
}

