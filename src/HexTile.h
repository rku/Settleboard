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

#ifndef HEX_TILE_H
#define HEX_TILE_H 1

#include <QtOpenGL>

#include "GLGameModelProxy.h"

class Game;
class NumberChip;
class Crossroad;
class Roadway;
class Robber;

class HexTile : public GLGameModelProxy
{
    Q_OBJECT

    public:
        enum HexTileType {NoType, WoodType, WaterType, WheatType,
            SheepType, DesertType, OreType, ClayType, GoldType};

        HexTile(HexTileType = NoType, unsigned int x = 0,
            unsigned int y = 0, QObject *parent = 0);
        ~HexTile();

        void setType(HexTileType);
        HexTileType getType();

        void setChipNumber(unsigned int number);
        unsigned int getChipNumber() { return chipNumber; }
        bool getHasNumberChip() { return (numberChip != NULL); }

        unsigned int getRow() { return position.y(); }
        unsigned int getColumn() { return position.x(); }

        void setHasFixedPosition(bool b) { hasFixedPosition = b; }
        bool getHasFixedPosition() { return hasFixedPosition; }
        void draw();

        void setHasRobber(bool);
        bool getHasRobber() { return (robber != NULL); }

        QVector3D getCenterVertex();
        QList<QVector3D> getCornerVertices();

        void addCrossroad(Crossroad*);
        const QList<Crossroad*>& getCrossroads() { return crossroads; }
        void addRoadway(Roadway*);
        const QList<Roadway*>& getRoadways() { return roadways; }

        const QString getResourceName() { return getResourceName(type); }
        static const QString getResourceName(HexTileType);

    private:
        void setColumn(unsigned int i) { position.setX(i); }
        void setRow(unsigned int i) { position.setY(i); }

        bool hasPort;
        bool hasFixedPosition;
        int chipNumber;
        Robber *robber;
        QPoint position;
        HexTileType type;
        NumberChip *numberChip;
        QList<Crossroad*> crossroads;
        QList<Roadway*> roadways;
};

Q_DECLARE_METATYPE(HexTile*);

class HexTilePtr
{
    public:
        HexTilePtr() : object(NULL) {}
        HexTilePtr(HexTile *p) { object = p; }
        HexTilePtr(const HexTilePtr &c) { object = c.object; }
        HexTile *getObject() { return object; }

        friend QDataStream &operator<<(QDataStream&, const HexTilePtr&);
        friend QDataStream &operator>>(QDataStream&, HexTilePtr&);

        HexTile *object;
};

Q_DECLARE_METATYPE(HexTilePtr);

#endif /* HEX_TILE_H */

