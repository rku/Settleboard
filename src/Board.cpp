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
#include "Roadway.h"

#include <math.h>

static GLGameModel *robber = NULL;

Board::Board(Game *_game) : game(_game)
{
    state               = BOARD_STATE_NORMAL;
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
    while(!boardTiles.isEmpty() ) delete boardTiles.takeFirst();
    while(!crossroads.isEmpty() ) delete crossroads.takeFirst();
    while(!roadways.isEmpty()   ) delete roadways.takeFirst();
}

void Board::render()
{
    for(int i = 0; i < boardTiles.size(); ++i)
        boardTiles.at(i)->draw();

    // render every single tile
    if(state == BOARD_STATE_NORMAL)
    {
        for(int i = 0; i < crossroads.size(); ++i)
            crossroads.at(i)->draw();

        for(int i = 0; i < roadways.size(); ++i)
            roadways.at(i)->draw();
    }

    if(state == BOARD_STATE_SET_BUILDING)
    {
        for(int i = 0; i < crossroads.size(); ++i)
            crossroads.at(i)->drawSelectionCircle();
    }

    if(state == BOARD_STATE_SET_ROAD)
    {
        for(int i = 0; i < roadways.size(); ++i)
            roadways.at(i)->drawSelectionRect();
    }

    if(!robber)
    {
        robber = new GLGameModel(game);
        robber->load("Data/Objects/robber.obj");
    }

    robber->draw();
}

void Board::getIndexOfTileAtMousePos(QPoint mousePos)
{
    QList<GLuint> hits;
    GLWidget *widget = game->getGLWidget();

    widget->beginGLSelection(mousePos);

    for(int i = 0; i < boardTiles.size(); ++i)
    {
        glLoadName(i);
        ((HexTile*)boardTiles.at(i))->draw();
    }

    hits = widget->endGLSelection();

    // test the picking mechanism by deleting clicked tiles
    // this ought to be removed when picking is implemented
    // fully

    for(int i = 0; i < hits.size(); ++i)
    {
        HexTile *tile = boardTiles.at(hits.at(i));

        if(robber) robber->setPos(tile->getCornerVertices()[0]);

        Crossroad *c = getCrossroadNearPosition(tile->getCornerVertices()[0]);
        qDebug() << "Crossroad of" << c->getTiles().size() << "tiles";
        for(int b=0; b<c->getTiles().size();b++)
        {
            qDebug() << "A tile of type" << c->getTiles().at(b)->getType();
        }
        widget->updateGL();

        break;
    }

    qDebug() << "tiles at mouse pos" << hits.size();
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

        // setup crossroads
        QList<Vertex3f> vertices = newTile->getCornerVertices();
        Q_ASSERT(vertices.size() == 6);
        for(int i = 0; i < vertices.size(); i++)
        {
            Crossroad *cr = getCrossroadNearPosition(vertices.at(i), true);
            cr->addTile(newTile);
        }

        // setup roadways
        vertices.append(vertices[0]);
        for(int i = 1; i < vertices.size(); i++)
        {
            // connect every corner of the tile
            getRoadwayNear(vertices.at(i-1), vertices.at(i), true);
        }
        vertices.removeLast();

        qDebug() << "ROADWAYS:" << roadways.size();

        // add tile
        boardTiles.insert(boardTiles.begin(), newTile);
    }

    qDebug() << "Board generated." << crossroads.size() << "crossroads" <<
        "and" << boardTiles.size() << "board tiles";
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

