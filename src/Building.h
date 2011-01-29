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

#ifndef BUILDING_H
#define BUILDING_H

#include "GLGameModel.h"

class Player;
class Game;
class Crossroad;
class Roadway;
class HexTile;

class Building : public GLGameModel
{
    public:
        Building(Game*, Player*, QString _type);

        virtual Player* getPlayer() { return player; }

        const QString& getType() { return type; }
        Crossroad *getCrossroad() { return crossroad; }
        void setCrossroad(Crossroad *c) { crossroad = c; }
        Roadway *getRoadway() { return roadway; }
        void setRoadway(Roadway *r) { roadway = r; }
        HexTile *getTile() { return tile; }
        void setTile(HexTile *t) { tile = t; }

    private:
        QString type;
        Player *player;
        Crossroad *crossroad;
        Roadway *roadway;
        HexTile *tile;
};

#endif

