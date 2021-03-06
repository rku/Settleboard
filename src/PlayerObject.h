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

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "GLGameModel.h"

class Player;
class Game;
class Crossroad;
class Roadway;
class HexTile;
class GLGameModelProxy;

class PlayerObject : public GLGameModel
{
    Q_OBJECT

    public:
        PlayerObject(Player*, QString _type, QObject *parent);

        virtual Player* getOwner() { return owner; }

        const QString& getType() { return type; }
        GLGameModelProxy *getBaseObject() { return baseObject; }
        void setBaseObject(GLGameModelProxy *c) { baseObject = c; }
        bool getIsPlaced() { return (baseObject != NULL); }
        HexTile *getTile() { return tile; }
        void setTile(HexTile *t) { tile = t; }

    protected:
        virtual void setupLightingParameters();

    private:
        QString type;
        Player *owner;
        GLGameModelProxy *baseObject;
        HexTile *tile;
        QPixmap *icon;
};

Q_DECLARE_METATYPE(PlayerObject*);

#endif

