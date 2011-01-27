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

#include "GameObject.h"
#include "HexTile.h"
#include "NumberChip.h"
#include "GLTypes.h"

class Game;
class Crossroad;
class Roadway;
class BoardState;

#define BOARD_MAX_TILES                 512 

#define BOARD_STATE_NORMAL              0x00
#define BOARD_STATE_SELECT_CROSSROAD    0x01
#define BOARD_STATE_SELECT_ROADWAY      0x02
#define BOARD_STATE_SELECT_HEXTILE      0x04

class Board : public GameObject
{
    public:
        Board(Game*);
        ~Board();
        void freeObjects();
        void render();
        void generate();
        bool loadFromFile(const QString&);
        bool loadByName(const QString&);

        void updateBoardState(BoardState&);

        void onMouseClick(QPoint mousePos);
        void onMouseOver(QPoint mousePos);

    protected:
        template <typename T> const T getObjectsAtMousePos(T, QPoint&);
        template <typename T> void highlightObjectsAtMousePos(T, QPoint&);
        Vertex3f getPosForTile(HexTile*, int col, int row);
        Crossroad *getCrossroadNearPosition(Vertex3f, bool create = false);
        Roadway *getRoadwayNear(Vertex3f, Vertex3f, bool create = false);

        QList<HexTile*> boardTiles;
        QList<NumberChip*> numberChips;
        unsigned int width;
        unsigned int height;
        unsigned int state;
        char tileData[BOARD_MAX_TILES];
        QList<Crossroad*> crossroads;
        QList<Roadway*> roadways;
        QString name;
        QString author;
        QString boardFilesPath;
        QString boardFilesSuffix;
};

#endif

