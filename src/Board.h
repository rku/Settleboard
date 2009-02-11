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

#ifndef BOARD_H
#define BOARD_H

#include <QtOpenGL>
#include <QList>

#include "HexTile.h"
#include "NumberChip.h"
#include "GLTypes.h"

class Game;

#define BOARD_MAX_TILES     512 

class Board
{
    public:
        Board(Game*);
        ~Board();
        void freeObjects();
        void render();
        void generate();
        bool loadFromFile(const QString&);
        bool loadByName(const QString&);

    protected:
        Vertex3f getPosForTileAt(int col, int row);

        Game *game;
        QList<HexTile*> boardTiles;
        QList<NumberChip*> numberChips;
        unsigned int width;
        unsigned int height;
        char tileData[BOARD_MAX_TILES];
        QString name;
        QString author;
        QString boardFilesPath;
        QString boardFilesSuffix;
};

#endif

