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
#include "CrossRoad.h"

#include <math.h>

Board::Board(Game *_game) : game(_game)
{
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
    while(!boardTiles.isEmpty()) delete boardTiles.takeFirst();
}

void Board::render()
{
    // render every single tile
    for(int i = 0; i < boardTiles.size(); ++i)
        ((HexTile*)boardTiles.at(i))->draw();

    // test robber :)
    GLGameModel *robber = new GLGameModel(game);
    robber->load("Data/Objects/robber.obj");
    robber->draw();
    delete robber;
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
        delete boardTiles.takeAt(hits.at(i));
        widget->updateGL();
        break;
        qDebug() << "Tile hit" << hits.at(i);
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

        pos = getPosForTile(newTile, col, row);
        newTile->setPos(pos.x, pos.y, pos.z);

        boardTiles.insert(boardTiles.begin(), newTile);
    }
}

