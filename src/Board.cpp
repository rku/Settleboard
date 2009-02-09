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

void Board::generate()
{
    GLfloat x,
            z,
            row = -1.0f,
            col = -1.0f;

    // discard old board
    freeObjects();

    for(char *p = tileData; row < height && *p; p++)
    {
        HexTile *newTile = new HexTile( game);

        col += 1.0f;
        if(col >= width)
        {
            col  = 0.0f;
            row += 1.0f;
        }

        switch(*p)
        {
            case 'X':
                newTile->setFixedPosition(true);
            case 'x':
                newTile->setType(HEX_TILE_TYPE_WATER);
                break;
            case 'E': // weed
                newTile->setFixedPosition(true);
            case 'e':
                newTile->setType(HEX_TILE_TYPE_WEED);
                break;
            case 'S': // sheep
                newTile->setFixedPosition(true);
            case 's':
                newTile->setType(HEX_TILE_TYPE_SHEEP);
                break;
            case 'W': // wood
                newTile->setFixedPosition(true);
            case 'w':
                newTile->setType(HEX_TILE_TYPE_WOOD);
                break;
            case 'C': // clay
                newTile->setFixedPosition(true);
            case 'c':
                newTile->setType(HEX_TILE_TYPE_CLAY);
                break;
            case 'G': // gold
                newTile->setFixedPosition(true);
            case 'g':
                newTile->setType(HEX_TILE_TYPE_GOLD);
                break;
            case 'O': // ore
                newTile->setFixedPosition(true);
            case 'o':
                newTile->setType(HEX_TILE_TYPE_ORE);
                break;
            case 'D': // desert
                newTile->setType(HEX_TILE_TYPE_DESERT);
                break;
            default:
                continue;
        }

        x =  1.0f + (-1.0f * width ) + (2.0f  * col) + ((int)row % 2);
        z = -0.5f + (-0.5f * height) + (1.75f * row);
        newTile->setPos(x, 0.0f, z);
        newTile->rotateY(90.0f);

        boardTiles.insert(boardTiles.begin(), newTile);
    }
}

