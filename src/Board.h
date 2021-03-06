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

#ifndef BOARD_H
#define BOARD_H

#include <QtOpenGL>
#include <QList>
#include <QPainter>

#include "HexTile.h"
#include "NumberChip.h"
#include "GameRules.h"
#include "GLWidget.h"
#include "GLTypes.h"
#include "BoardState.h"
#include "Map.h"

class Game;
class Crossroad;
class Roadway;

class Board : public QObject
{
    Q_OBJECT

    public:
        Board(QObject *parent = 0);
        ~Board();
        void reset();
        void freeObjects();
        void update();
        bool load(Map*);
        Map* getMap() { return map; }
        bool getIsLoaded() { return isLoaded; }

        void resetBoardState(BoardObjectState s = defaultBoardObjectState);
        void updateBoardState(BoardState&);

        // selection
        void setSelectionMode();
        void endSelectionMode();
        bool getIsSelectionModeActive() { return isSelectionModeActive; }
        bool getHasSelectedObject() { return (selectedObject != NULL); }
        GLGameModel *getSelectedObject() { return selectedObject; }
        void setSelectedObject(GLGameModel*);

        const QList<HexTile*> getBoardTiles() { return map->getTiles(); }
        const QList<Crossroad*> getCrossroads() { return crossroads; }
        const QList<Roadway*> getRoadways() { return roadways; }

    public slots:
        void render();
        void handleMouseClick(QMouseEvent*);
        void handleMouseOver(QMouseEvent*);

    protected:
        template <typename T> const T getObjectsAtMousePos(T, const QPoint&);
        template <typename T> void highlightObjectsAtMousePos(T, const QPoint&);
        GLGameModel *getSelectableObjectAtMousePos(const QPoint &pos);
        void setPosForTile(HexTile*);
        Crossroad *getCrossroadNearPosition(QVector3D, bool create = false);
        Roadway *getRoadwayNear(QVector3D, QVector3D, bool create = false);

        Map *map;
        bool isSelectionModeActive;
        bool isLoaded;
        GLGameModel *selectedObject;
        QList<Crossroad*> crossroads;
        QList<Roadway*> roadways;
};

#endif

