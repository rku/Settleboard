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
#include <QPainter>

#include "HexTile.h"
#include "NumberChip.h"
#include "GLTypes.h"
#include "BoardState.h"

class Game;
class Crossroad;
class Roadway;

#define BOARD_MAX_TILES                 512 

#define BOARD_STATE_NORMAL              0x00
#define BOARD_STATE_SELECT_CROSSROAD    0x01
#define BOARD_STATE_SELECT_ROADWAY      0x02
#define BOARD_STATE_SELECT_HEXTILE      0x04

class Board : public QObject
{
    Q_OBJECT

    public:
        Board(QObject *parent = 0);
        ~Board();
        void freeObjects();
        void render();
        void generate();
        void update();
        bool loadFromFile(const QString&);
        bool loadByName(const QString&);
        bool getIsLoaded() { return isLoaded; }

        void resetBoardState(BoardObjectState s = defaultBoardObjectState);
        void updateBoardState(BoardState&);

        bool handleMouseClick(const QPoint &mousePos);
        bool handleMouseOver(const QPoint &mousePos);

        // selection
        void setSelectionMode();
        void endSelectionMode();
        bool getIsSelectionModeActive() { return isSelectionModeActive; }
        bool getHasSelectedObject() { return (selectedObject != NULL); }
        GLGameModel *getSelectedObject() { return selectedObject; }
        void setSelectedObject(GLGameModel*);

        const QList<HexTile*> getBoardTiles() { return boardTiles; }
        const QList<Crossroad*> getCrossroads() { return crossroads; }
        const QList<Roadway*> getRoadways() { return roadways; }

    protected:
        template <typename T> const T getObjectsAtMousePos(T, const QPoint&);
        template <typename T> void highlightObjectsAtMousePos(T, const QPoint&);
        GLGameModel *getSelectableObjectAtMousePos(const QPoint &pos);
        QVector3D getPosForTile(HexTile*, int col, int row);
        Crossroad *getCrossroadNearPosition(QVector3D, bool create = false);
        Roadway *getRoadwayNear(QVector3D, QVector3D, bool create = false);

        QList<HexTile*> boardTiles;
        QList<NumberChip*> numberChips;
        unsigned int width;
        unsigned int height;
        char tileData[BOARD_MAX_TILES];
        bool isSelectionModeActive;
        bool isLoaded;
        GLGameModel *selectedObject;
        QList<Crossroad*> crossroads;
        QList<Roadway*> roadways;
        QString name;
        QString author;
};

#endif

