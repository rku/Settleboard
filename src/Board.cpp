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
#include "Roadway.h"

#include <math.h>

Board::Board(Game *_game) : GameObject(_game)
{
    state               = BOARD_STATE_SELECT_CROSSROAD;
    boardFilesPath      = "Data/Boards/";
    boardFilesSuffix    = ".rsm";
}

Board::~Board()
{
    freeObjects();
}

void Board::freeObjects()
{
    while(!numberChips.isEmpty()) delete numberChips.takeFirst();
    while(! boardTiles.isEmpty()) delete boardTiles.takeFirst();
    while(! crossroads.isEmpty()) delete crossroads.takeFirst();
    while(!   roadways.isEmpty()) delete roadways.takeFirst();
}

void Board::render()
{
    for(int i = 0; i < boardTiles.size(); ++i)
        boardTiles.at(i)->draw();

    for(int i = 0; i < crossroads.size(); ++i)
        crossroads.at(i)->draw();

    for(int i = 0; i < roadways.size(); ++i)
        roadways.at(i)->draw();

    GLGameModel *robber = new GLGameModel(game);
    robber->load("Data/Objects/robber.obj");
    robber->setColor(Qt::black);
    robber->draw();
    delete robber;
}

/* returns the glgamemodel objects in objs at 2d mouse position pos*/
template <typename T>
const T Board::getObjectsAtMousePos(T objs, QPoint &pos)
{
    QList<GLuint> hits;
    GLWidget *widget = game->getGLWidget();

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
void Board::highlightObjectsAtMousePos(T objs, QPoint &pos)
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

void Board::onMouseClick(QPoint mousePos)
{
    BoardState state;
    BoardObjectState s;

    s.index = 1;
    s.selectable = true;

    state.crossroadStates.append(s);
    state.roadwayStates.append(s);

    qDebug() << "updating state...";
    updateBoardState(state);
}

void Board::onMouseOver(QPoint mousePos)
{
    // highlight selectable objects at mousepos
    highlightObjectsAtMousePos(crossroads, mousePos);
    highlightObjectsAtMousePos(roadways, mousePos);
    highlightObjectsAtMousePos(boardTiles, mousePos);

    game->getGLWidget()->updateGL();
}

void Board::updateBoardState(BoardState &newState)
{
    int max = 1;
    BoardObjectState s;

    for(int i = 0; i < max; i++)
    {
        bool done = true;

        if(newState.tileStates.size() >= max)
        {
            s = newState.tileStates.at(i);
            done = false;
        }

        if(newState.roadwayStates.size() >= max)
        {
            s = newState.roadwayStates.at(i);
            Roadway *r = roadways.at(s.index);
            r->setIsSelectable(s.selectable);
            r->setIsEnabled(s.enabled);
            done = false;
        }

        if(newState.crossroadStates.size() >= max)
        {
            s = newState.crossroadStates.at(i);
            Crossroad *c = crossroads.at(s.index);
            c->setIsSelectable(s.selectable);
            c->setIsEnabled(s.enabled);
            done = false;
        }

        if(!done) max++;
    }

    game->getGLWidget()->updateGL();
}

// load a board from a plain textfile
// See MAPFORMAT for a description of the
// file format.
bool Board::loadFromFile(const QString& filename)
{
    QFile boardFile(filename);
    QTextStream stream(&boardFile);
    char *pDir = tileData;

    memset(pDir, 0, BOARD_MAX_TILES);

    if(!boardFile.open(QFile::ReadOnly))
    {
        qCritical() << "Could'nt load board file: " << filename;
        return false;
    }

    name   = stream.readLine();
    author = stream.readLine();

    stream >> width;
    stream >> height;

    if((width * height) > (BOARD_MAX_TILES-1))
    {
        qCritical() << "Board too large.";
        return false;
    }

    if(width < 2 || height < 2)
    {
        qCritical() << "Boardsize must be at least 2x2.";
        return false;
    }

    for(unsigned int i = 0; i < (width * height); i++)
    {
        if(stream.atEnd())
        {
            qCritical() << "Can't load board. Invalid data size.";
            return false;
        }

        stream >> *pDir;

        if(*pDir == '\n' || *pDir == ' ')
            { i--; } else { pDir++; }
    }

    qDebug() << "Board loaded: " << filename;

    boardFile.close();

    return true;
}

bool Board::loadByName(const QString &name)
{
    QString path = QString(name);

    path.insert(0, boardFilesPath);
    path.append(boardFilesSuffix);

    return loadFromFile(path);
}

Vertex3f Board::getPosForTile(HexTile *tile, int col, int row)
{
    Vertex3f pos;
    float w = tile->getWidth();
    float d = tile->getDepth() - 0.6f;

    // center - boardwidth/2 + tilewidth*col
    pos.x = 0 - ( width * (w/2) ) + (col * w) + (row % 2) * (w/2);
    pos.y = 0.0f;
    // center - boardheight/2 - tiledepth/2 + row*tiledepth
    pos.z = (d/2) - ( height * (d/2) ) + (row * d);

    return pos;
}

void Board::generate()
{
    int row = 0, col = -1;
    Vertex3f pos;

    // discard old board
    freeObjects();

    for(char *p = tileData; row < (int)height && *p; p++)
    {
        HexTile *newTile = new HexTile(game);

        col++;
        if(col >= (int)width)
        {
            col = 0;
            row++;
        }

        switch(*p)
        {
            case 'X':
                newTile->setFixedPosition(true);
            case 'x':
                newTile->setType(HEXTILE_TYPE_WATER);
                break;
            case 'E': // weed
                newTile->setFixedPosition(true);
            case 'e':
                newTile->setType(HEXTILE_TYPE_WEED);
                break;
            case 'S': // sheep
                newTile->setFixedPosition(true);
            case 's':
                newTile->setType(HEXTILE_TYPE_SHEEP);
                break;
            case 'W': // wood
                newTile->setFixedPosition(true);
            case 'w':
                newTile->setType(HEXTILE_TYPE_WOOD);
                break;
            case 'C': // clay
                newTile->setFixedPosition(true);
            case 'c':
                newTile->setType(HEXTILE_TYPE_CLAY);
                break;
            case 'G': // gold
                newTile->setFixedPosition(true);
            case 'g':
                newTile->setType(HEXTILE_TYPE_GOLD);
                break;
            case 'O': // ore
                newTile->setFixedPosition(true);
            case 'o':
                newTile->setType(HEXTILE_TYPE_ORE);
                break;
            case 'D': // desert
                newTile->setType(HEXTILE_TYPE_DESERT);
                break;
            default:
                continue;
        }

        // set position
        pos = getPosForTile(newTile, col, row);
        newTile->setPos(pos.x, pos.y, pos.z);

        // setup crossroads and roadways
        QList<Vertex3f> vertices = newTile->getCornerVertices();
        Q_ASSERT(vertices.size() == 6);
        vertices.append(vertices[0]);
        for(int i = 1; i < vertices.size(); i++)
        {
            Crossroad *cr = getCrossroadNearPosition(vertices.at(i-1), true);
            cr->addTile(newTile);

            Roadway *rw = getRoadwayNear(vertices.at(i-1), vertices.at(i), true);
            rw->addTile(newTile);
            rw->addCrossroad(cr);
        }

        // add tile
        boardTiles.insert(boardTiles.begin(), newTile);
    }

    qDebug() << "Board generated:" << crossroads.size() << "crossroads" <<
        "," << roadways.size() << "roadways" <<
        "and" << boardTiles.size() << "tiles";
}

Crossroad *Board::getCrossroadNearPosition(Vertex3f pos, bool create)
{
    for(int i = 0; i < crossroads.size(); i++)
    {
        Crossroad *a = crossroads.at(i);
        Vertex3f v = a->getVertex();

        v.x -= pos.x; v.y -= pos.y; v.z -= pos.z;
        if(qAbs(v.x) < 0.1f && qAbs(v.y) < 0.1f && qAbs(v.z) < 0.1f) return a;
    }

    // not found
    Crossroad *c = NULL;

    if(create)
    {
        c = new Crossroad(game, pos);
        crossroads.append(c);
    }

    return c;
}

Roadway *Board::getRoadwayNear(Vertex3f a, Vertex3f b, bool create)
{
    for(int i = 0; i < roadways.size(); i++)
    {
        Roadway *r = roadways.at(i);
        Vertex3f ra = r->getVertices().at(0);
        Vertex3f rb = r->getVertices().at(1);

#define COMP_ROADWAY_VECS(c,d) if(1) { \
    if(qAbs(a.x-c.x) + qAbs(a.y-c.y) + qAbs(a.z-c.z) + \
        qAbs(b.x-d.x) + qAbs(b.y-d.y) + qAbs(b.z-d.z) < 0.5f) return r; }

        COMP_ROADWAY_VECS(ra, rb);
        COMP_ROADWAY_VECS(rb, ra);
    }

    Roadway *newR = NULL;

    if(create)
    {
        newR = new Roadway(game, a, b);
        roadways.append(newR);
    }

    return newR;
}

