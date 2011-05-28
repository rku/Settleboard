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

#ifndef CROSSROAD_H
#define CROSSROAD_H 1

#include <QObject>

#include "GLGameModelProxy.h"
#include "GLTypes.h"

class HexTile;
class Game;
class Roadway;
class PlayerObject;

class Crossroad : public GLGameModelProxy
{
    Q_OBJECT

    public:
        Crossroad(QVector3D, QObject *parent = 0);
        ~Crossroad();

        virtual void draw();

        QVector3D getVertex() { return vertex; }
        void setVertex(QVector3D);

        const QList<HexTile*> getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Crossroad*> getNeighbours() { return neighbours; }
        void addNeighbour(Crossroad*);

        const QList<Roadway*> getRoadways() { return roadways; }
        void addRoadway(Roadway*);

        void placePlayerObject(PlayerObject*);

        const QVector3D& getPos() { return vertex; }

    private:
        void createSelectionCircle();
        void drawSelectionCircle();

        GLuint selectionCircleListID;
        QVector3D vertex;
        qreal angleToOrigin;
        QList<HexTile*> tiles;
        QList<Crossroad*> neighbours;
        QList<Roadway*> roadways;
};

Q_DECLARE_METATYPE(Crossroad*);

class CrossroadPtr
{
    public:
        CrossroadPtr() : object(NULL) {}
        CrossroadPtr(Crossroad *p) { object = p; }
        CrossroadPtr(const CrossroadPtr &c) { object = c.object; }
        Crossroad *getObject() { return object; }

        friend QDataStream &operator<<(QDataStream&, const CrossroadPtr&);
        friend QDataStream &operator>>(QDataStream&, CrossroadPtr&);

        Crossroad *object;
};

Q_DECLARE_METATYPE(CrossroadPtr);

#endif

