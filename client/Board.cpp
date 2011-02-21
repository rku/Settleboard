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

#include "Board.h"
#include "Game.h"
#include "Crossroad.h"
#include "BoardState.h"
#include "FileManager.h"
#include "GLWidget.h"
#include "StandardMap.h"
#include "Roadway.h"

#include <math.h>

Board::Board(QObject *parent) : QObject(parent)
{
    map                     = NULL;
    isLoaded                = false;
    isSelectionModeActive   = false;
    selectedObject          = NULL;
}

Board::~Board()
{
    freeObjects();
}

void Board::reset()
{
    freeObjects();

    isLoaded = false;
    isSelectionModeActive = false;
    selectedObject = NULL;

    update();
}

void Board::freeObjects()
{
    while(! crossroads.isEmpty()) delete crossroads.takeFirst();
    while(!   roadways.isEmpty()) delete roadways.takeFirst();

    if(map) delete map;
    map = NULL;
}

void Board::render()
{
    if(!getIsLoaded()) return;

    for(int i = 0; i < getBoardTiles().size(); ++i)
        getBoardTiles().at(i)->draw();

    for(int i = 0; i < crossroads.size(); ++i)
        crossroads.at(i)->draw();

    for(int i = 0; i < roadways.size(); ++i)
        roadways.at(i)->draw();
}

void Board::update()
{
    GAME->getGLWidget()->updateGL();
}

/* returns the glgamemodel objects in objs at 2d mouse position pos*/
template <typename T>
const T Board::getObjectsAtMousePos(T objs, const QPoint &pos)
{
    QList<GLuint> hits;
    GLWidget *widget = GAME->getGLWidget();

    widget->beginGLSelection(pos);

    for(int i = 0; i < objs.size(); i++)
    {
        glLoadName(i);
        objs.at(i)->draw();
    }

    hits = widget->endGLSelection();

    T result;
    for(int i = 0; i < hits.size(); i++) result.append(objs.at(hits.at(i)));
    return result;
}

template <typename T>
void Board::highlightObjectsAtMousePos(T objs, const QPoint &pos)
{
    T selObjs;

    for(int i = 0; i < objs.size(); i++)
    {
        objs.at(i)->setIsHighlighted(false);
        if(objs.at(i)->getIsSelectable()) selObjs.append(objs.at(i));
    }

    T hits = getObjectsAtMousePos(selObjs, pos);
    if(hits.size())
        for(int i = 0; i < hits.size(); i++)
            hits.at(i)->setIsHighlighted(true);
}

GLGameModel *Board::getSelectableObjectAtMousePos(const QPoint &pos)
{
    QList<GLGameModel*> selObjs, hits;
    int max = 1;

#define ADD_SELECTABLE_OBJECTS(a) if(a.size()>i) {\
        GLGameModel *obj = dynamic_cast<GLGameModel*>(a.at(i)); \
        if(obj->getIsSelectable()) selObjs.append(obj); \
        done = false; }

    for(int i = 0; i < max; i++)
    {
        bool done = true;

        ADD_SELECTABLE_OBJECTS(crossroads);
        ADD_SELECTABLE_OBJECTS(roadways);
        ADD_SELECTABLE_OBJECTS(getBoardTiles());

        if(!done) max++;
    }

    hits = getObjectsAtMousePos(selObjs, pos);
    return (hits.size()>0) ? hits.at(0) : NULL;
}

void Board::handleMouseClick(QMouseEvent *event)
{
    if(!isLoaded) return;

    const QPoint mousePos = event->pos();

    if(getIsSelectionModeActive())
    {
        GLGameModel *obj = getSelectableObjectAtMousePos(mousePos);
        if(obj != NULL)
        {
            setSelectedObject(obj);
            endSelectionMode();

            GAME->getRules()->pushRuleData(obj);
            GAME->getRules()->executeRule("ruleBoardObjectSelected");
        }
    }
}

void Board::handleMouseOver(QMouseEvent *event)
{
    if(!isLoaded) return;

    const QPoint mousePos = event->pos();

    // highlight selectable objects at mousepos
    highlightObjectsAtMousePos(crossroads, mousePos);
    highlightObjectsAtMousePos(roadways, mousePos);
    highlightObjectsAtMousePos(getBoardTiles(), mousePos);

    GAME->getGLWidget()->updateGL();
}

void Board::setSelectionMode()
{
    Q_ASSERT(isLoaded);
    Q_ASSERT(!getIsSelectionModeActive());

    int max = 1;

    // disable all board objects except the selectable ones
    for(int i = 0; i < max; i++)
    {
        bool done = true;

#define DISABLE_NONSELECTABLE_OBJECT_OF(a) \
    if(a.size() > i) { \
        a.at(i)->setIsEnabled(a.at(i)->getIsSelectable()); done = false; }

        DISABLE_NONSELECTABLE_OBJECT_OF(crossroads);
        DISABLE_NONSELECTABLE_OBJECT_OF(roadways);
        DISABLE_NONSELECTABLE_OBJECT_OF(getBoardTiles());

        if(!done) max++;
    }

    selectedObject = NULL;
    isSelectionModeActive = true;
    update();
}

void Board::endSelectionMode()
{
    Q_ASSERT(getIsSelectionModeActive());

    isSelectionModeActive = false;
    resetBoardState();

    selectedObject = NULL;
}

void Board::setSelectedObject(GLGameModel *gm)
{
    selectedObject = gm;
}

void Board::resetBoardState(BoardObjectState s)
{
    Q_ASSERT(isLoaded);

    BoardState state;

#define CREATE_RESET_STATE_FOR(objs, sto) if(1) { \
    for(int i = 0; i < objs.size(); i++) { \
        s.index = i; \
        state.sto.append(s); } }

    CREATE_RESET_STATE_FOR(getBoardTiles(), tileStates);
    CREATE_RESET_STATE_FOR(crossroads, crossroadStates);
    CREATE_RESET_STATE_FOR(roadways, roadwayStates);

    updateBoardState(state);
}

void Board::updateBoardState(BoardState &newState)
{
    Q_ASSERT(isLoaded);

    int max = 1;
    BoardObjectState s;

    for(int i = 0; i < max; i++)
    {
        bool done = true;

#define UPDATE_GLOBJECT_STATE(obj, state) if(1) { \
    obj->setIsSelectable(s.selectable); \
    obj->setIsEnabled(s.enabled); \
    done = false; }

        if(newState.tileStates.size() >= max)
        {
            s = newState.tileStates.at(i);
            HexTile *h = getBoardTiles().at(s.index);
            UPDATE_GLOBJECT_STATE(h, s);
        }

        if(newState.roadwayStates.size() >= max)
        {
            s = newState.roadwayStates.at(i);
            Roadway *r = roadways.at(s.index);
            UPDATE_GLOBJECT_STATE(r, s);
        }

        if(newState.crossroadStates.size() >= max)
        {
            s = newState.crossroadStates.at(i);
            Crossroad *c = crossroads.at(s.index);
            UPDATE_GLOBJECT_STATE(c, s);
        }

        if(!done) max++;
    }

    GAME->getGLWidget()->updateGL();
}

void Board::setPosForTile(HexTile *tile)
{
    float w = tile->getWidth();
    float d = tile->getDepth() * 0.75f;
    unsigned int row = tile->getRow();
    unsigned int col = tile->getColumn();
    unsigned int width = map->getWidth();
    unsigned int height = map->getHeight();

    tile->setPos(QVector3D(
        // center - boardwidth/2 + tilewidth*col
        0 - ( width * (w/2) ) + (col * w) + (row % 2) * (w/2),
        0.0f,
        // center - boardheight/2 - tiledepth/2 + row*tiledepth
        (d/2) - ( height * (d/2) ) + (row * d)));
}

bool Board::load(Map *theMap)
{
    // discard old board
    freeObjects();

    map = theMap;

    HexTileList tiles = map->getTiles();
    HexTileList::iterator i;

    for(i = tiles.begin(); i != tiles.end(); ++i)
    {
        HexTile *newTile = *i;

        // set position
        setPosForTile(newTile);

        // setup crossroads and roadways
        QList<QVector3D> vertices = newTile->getCornerVertices();
        Q_ASSERT(vertices.size() == 6);
        vertices.append(vertices[0]);
        Crossroad *lastCrossroad = NULL;
        for(int i = 1; i < vertices.size(); i++)
        {
            Crossroad *cr = getCrossroadNearPosition(vertices.at(i-1), true);
            cr->addTile(newTile);

            if(lastCrossroad != NULL) cr->addNeighbour(lastCrossroad);

            Roadway *rw = getRoadwayNear(vertices.at(i-1), vertices.at(i), true);
            rw->addTile(newTile);
            rw->addCrossroad(cr);

            lastCrossroad = cr;
        }
    }

    qDebug() << "Board generated:" << crossroads.size() << "crossroads" <<
        "," << roadways.size() << "roadways" <<
        "and" << getBoardTiles().size() << "tiles";

    isLoaded = true;
    return true;
}

Crossroad *Board::getCrossroadNearPosition(QVector3D pos, bool create)
{
    for(int i = 0; i < crossroads.size(); i++)
    {
        Crossroad *a = crossroads.at(i);
        QVector3D v = a->getVertex();

        if(qAbs((v - pos).length()) < 0.1) return a;
    }

    // not found
    Crossroad *c = NULL;

    if(create)
    {
        c = new Crossroad(pos, this);
        crossroads.append(c);
    }

    return c;
}

Roadway *Board::getRoadwayNear(QVector3D a, QVector3D b, bool create)
{
    for(int i = 0; i < roadways.size(); i++)
    {
        Roadway *r = roadways.at(i);
        QVector3D ra = r->getVertexA();
        QVector3D rb = r->getVertexB();

#define COMP_ROADWAY_VECS(c,d) if(1) { \
    if(qAbs((a - c).length()) + \
        qAbs((b - d).length()) < 0.2) return r; }

        COMP_ROADWAY_VECS(ra, rb);
        COMP_ROADWAY_VECS(rb, ra);
    }

    Roadway *newR = NULL;

    if(create)
    {
        newR = new Roadway(a, b, this);
        roadways.append(newR);
    }

    return newR;
}

