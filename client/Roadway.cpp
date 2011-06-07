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
#include "Player.h"
#include "Crossroad.h"
#include "PlayerObject.h"
#include "Roadway.h"

Roadway::Roadway(QVector3D a, QVector3D b, QObject *parent)
    : GLGameModelProxy(parent)
{
    playerObject = NULL;

    setVertices(a,b);

    selectionRectListID = glGenLists(1);
    createSelectionRect();
}

Roadway::~Roadway()
{
    glDeleteLists(selectionRectListID, 1);
}

void Roadway::createSelectionRect()
{
    glNewList(selectionRectListID, GL_COMPILE);

    glTranslatef(0.0f, 0.0001f, 0.0f);

    glBegin(GL_QUADS);

    glVertex3f(vertexA.x(), vertexA.y(), vertexA.z());
    glVertex3f(vertexB.x(), vertexB.y(), vertexB.z());
    glVertex3f(vertexC.x(), vertexC.y(), vertexC.z());
    glVertex3f(vertexD.x(), vertexD.y(), vertexD.z());

    glEnd();

    glEndList();
}

void Roadway::draw()
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->draw();
        return;
    }

    if(getIsSelectable() && GAME->getBoard()->getIsSelectionModeActive())
    {
        QColor col = GAME->getLocalPlayer()->getColor();
        float width = 5.0f;

        // draw roadway lines
        glPushMatrix();
        GAME->getGLWidget()->qglColor((getIsHighlighted()) ? col.lighter() : col);
        glLineWidth(width);
        glCallList(selectionRectListID);
        glPopMatrix();
    }
}

void Roadway::setVertices(QVector3D a, QVector3D b)
{
    // always start with the vector having the lowest x value
    if(b.x() < a.x()) qSwap(a,b); 

    // calculate vertices for a selection rectangle of this roadway

    // create vector representing the line we want to draw our rect at
    QVector3D ab = b - a;

    // calculate a vector that stands orthogonal on vector ab 
    //
    // the idea is to use a plane ab is a normal vector for
    // then set y to 0, x (randomly chosen) to ab.x() and calculate
    // the corresponding z value in that plane - the resulting vector
    // has the right direction and can be normalized to a length of 1
    //
    // the plane we use is: x*ab.x() + y*ab.y() + z*ab.z() = 0
    // since we have set a value for x and y we can calculate z easily:
    Q_ASSERT(ab.z() != 0);
    QVector3D d(ab.x(), 0, - (ab.x() * ab.x()) / ab.z());
    d.normalize();

    // now use this vector to create vertices A,B,C,D (rect corners)
    // 
    //      C<--b-->B
    //         /
    //        /                  the vector --> is width *  d
    //       /                              <-- is width * -d
    //      /
    // D<--a-->A
    //
    qreal width = ab.length() / 16;
    vertexA = a + width*d;
    vertexB = b + width*d;
    vertexC = b - width*d;
    vertexD = a - width*d;

    // we have to make sure that the vertices appear in counterclockwise
    // order since this is required for opengl to draw a front face
    // if they dont, we swap them
    //
    // since we are only interested in the sign of the area, so we use a
    // simplified version of the gaussian trapeze formula (we do not divide
    // the area by 2 and we only use 3 vertices instead of all 4)
    //
    // a positive value for A means vertices are ordered clockwise
    qreal A = (vertexA.z()+vertexB.z())*(vertexA.x()-vertexB.x()) +
              (vertexB.z()+vertexC.z())*(vertexB.x()-vertexC.x()) +
              (vertexC.z()+vertexA.z())*(vertexC.x()-vertexA.x());
    if(A >= 0)
    {
        qSwap(vertexA, vertexB);
        qSwap(vertexC, vertexD);
    }

    // calculate center between a and b, where objects should be placed
    // coords of centerVertex: vector(0->a) + 0.5*vector(a->b)
    centerVertex = a + 0.5 * ab;
}

void Roadway::addTile(HexTile *tile)
{
    if(tiles.contains(tile)) return;

    Q_ASSERT(tiles.size() < 2);
    tiles.append(tile);
    tile->addRoadway(this);
}

void Roadway::addNeighbour(Roadway *roadway)
{
    if(neighbours.contains(roadway)) return;

    Q_ASSERT(neighbours.size() < 4);
    neighbours.append(roadway);
    roadway->addNeighbour(this);
}

void Roadway::addCrossroad(Crossroad *crossroad)
{
    if(crossroads.contains(crossroad)) return;

    Q_ASSERT(crossroads.size() < 2);
    crossroads.append(crossroad);
    crossroad->addRoadway(this);
}

void Roadway::placePlayerObject(PlayerObject *p)
{
    GLGameModelProxy::placePlayerObject(p);

    if(playerObject != NULL)
    {
        playerObject->pointInDirection(vertexB - vertexA);
        playerObject->setPos(centerVertex);
    }
}

// QDataStream operators

QDataStream &operator<<(QDataStream &stream, const RoadwayPtr &obj)
{
    int index = GAME->getBoard()->getRoadways().indexOf(obj.object);
    Q_ASSERT(index >= 0);
    stream << (quint16)index;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, RoadwayPtr &obj)
{
    quint16 id;
    stream >> id;

    // find object
    Q_ASSERT(id < GAME->getBoard()->getRoadways().size());
    obj.object = GAME->getBoard()->getRoadways().at(id);

    return stream;
}

