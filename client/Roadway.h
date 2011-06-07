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

#ifndef ROADWAY_H
#define ROADWAY_H 1

#include "GLGameModelProxy.h"

#include "GLTypes.h"

class HexTile;
class Game;
class Crossroad;
class PlayerObject;

class Roadway : public GLGameModelProxy
{
    Q_OBJECT

    public:
        Roadway(QVector3D a, QVector3D b, QObject *parent = 0);
        ~Roadway();

        virtual void draw();

        const QVector3D& getVertexA() { return vertexA; }
        const QVector3D& getVertexB() { return vertexB; }
        void setVertices(QVector3D a, QVector3D b);

        const QList<HexTile*>& getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Roadway*>& getNeighbours() { return neighbours; }
        void addNeighbour(Roadway*);

        const QList<Crossroad*>& getCrossroads() { return crossroads; }
        void addCrossroad(Crossroad*);

        void placePlayerObject(PlayerObject*);

        const QVector3D& getPos() { return centerVertex; }

    private:
        void createSelectionRect();

        QVector3D centerVertex;
        QVector3D objectDirection;
        QVector3D vertexA;
        QVector3D vertexB;
        QVector3D vertexC;
        QVector3D vertexD;
        GLuint selectionRectListID;
        QList<HexTile*> tiles;
        QList<Roadway*> neighbours;
        QList<Crossroad*> crossroads;
};

Q_DECLARE_METATYPE(Roadway*);

class RoadwayPtr
{
    public:
        RoadwayPtr() : object(NULL) {}
        RoadwayPtr(Roadway *p) { object = p; }
        RoadwayPtr(const RoadwayPtr &c) { object = c.object; }
        Roadway *getObject() { return object; }

        friend QDataStream &operator<<(QDataStream&, const RoadwayPtr&);
        friend QDataStream &operator>>(QDataStream&, RoadwayPtr&);

        Roadway *object;
};

Q_DECLARE_METATYPE(RoadwayPtr);

#endif

