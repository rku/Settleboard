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

#include "Game.h"
#include "Crossroad.h"
#include "NumberChip.h"
#include "Roadway.h"
#include "FileManager.h"
#include "Robber.h"
#include "HexTile.h"

HexTile::HexTile(HexTileType t, unsigned int x, unsigned int y,
    QObject *parent) : GLGameModelProxy(parent)
{
    setColumn(x);
    setRow(y);

    // random rotation (rotate in 60 degree steps)
    float angle = 60.0 * qRound((qrand() * 5.0) / RAND_MAX);
    setAngleY(angle);

    numberChip = NULL;
    robber = NULL;
    chipNumber = 0;
    setType(t);
}

HexTile::~HexTile()
{
    if(numberChip) delete numberChip;
    if(robber) delete robber;
}

void HexTile::setChipNumber(unsigned int number)
{
    Q_ASSERT(number > 1 && number < 13 && number != 7);

    QString texture("nc%1.jpg");
    setTexture("number", texture.arg(number));
    if(!numberChip) numberChip = new NumberChip(this);
    numberChip->setNumber(number);
    chipNumber = number;
}

void HexTile::setType(HexTileType t)
{
    QColor color;
    QString surfaceTexture;

    if(t == NoType)
    {
        type = t;
        return;
    }

    switch(t)
    {
        case WaterType:
            surfaceTexture = "water.jpg";
            color.setRgb(22,73,142,255);
            break;
        case DesertType:
            surfaceTexture = "desert.jpg";
            color.setRgb(225,255,255,255);
            break;
        case WoodType:
            surfaceTexture = "wood.jpg";
            color.setRgb(27,150,11,255);
            break;
        case SheepType:
            surfaceTexture = "sheep.jpg";
            color.setRgb(208,241,206,255);
            break;
        case WheatType:
            surfaceTexture = "wheat.jpg";
            color.setRgb(237,239,97,255); 
            break;
        case OreType:
            surfaceTexture = "ore.jpg";
            color.setRgb(106,112,124,255);
            break;
        case GoldType:
            surfaceTexture = "gold.jpg"; 
            color.setRgb(222,224,35,255);
            break;
        case ClayType:
            surfaceTexture = "clay.jpg";
            color.setRgb(231,126,33,255);
            break;
        default:
            qDebug() << "Unknown hextile type:" << t;
            return;
    }

    type = t;
    setTexture("surface", surfaceTexture);
    load(FileManager::getPathOfGLObject("hextile"), color);
}

void HexTile::draw()
{
    if(getType() == NoType) return;

    GLGameModel::draw();

    if(numberChip)
    {
        QVector3D v = getCenterVertex();
        numberChip->setPosX(v.x());
        numberChip->setPosZ(v.z());
        numberChip->draw();
    }

    if(robber) robber->draw();
}

void HexTile::setHasRobber(bool b)
{
    if(b && (robber == NULL))
    {
        robber = new Robber(this);
        robber->setScale(0.6);
        robber->setPos(getCenterVertex());
        return;
    }

    if(!b && (robber != NULL))
    {
        delete robber;
        robber = NULL;
    }
}

HexTile::HexTileType HexTile::getType()
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

const QString HexTile::getResourceName(HexTileType t)
{
    switch(t)
    {
        case OreType:    return "Ore";
        case GoldType:   return "Gold";
        case WheatType:  return "Wheat";
        case SheepType:  return "Wool";
        case WoodType:   return "Lumber";
        case ClayType:   return "Clay";
        default: return QString();
    }
}

// QDataStream operators

QDataStream &operator<<(QDataStream &stream, const HexTilePtr &obj)
{
    QList<HexTile*> tiles = GAME->getBoard()->getBoardTiles();

    Q_ASSERT(tiles.contains(obj.object));
    stream << (qint16)tiles.indexOf(obj.object);

    return stream;
}

QDataStream &operator>>(QDataStream &stream, HexTilePtr &obj)
{
    QList<HexTile*> tiles = GAME->getBoard()->getBoardTiles();

    qint16 index;
    stream >> index;

    Q_ASSERT(index >= 0 && index < tiles.size());
    obj.object = tiles.at(index);

    return stream;
}
