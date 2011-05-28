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

#include "Game.h"
#include "Player.h"
#include "GLGameModelProxy.h"
#include "FileManager.h"
#include "Crossroad.h"
#include "Roadway.h"
#include "PlayerObject.h"

PlayerObject::PlayerObject(Player *_owner, QString _type, QObject *parent)
    : GLGameModel(parent), type(_type), owner(_owner)
{
    load(FileManager::getPathOfGLObject(type));
    setColor(owner->getColor());

    baseObject = NULL;
}

void PlayerObject::setupLightingParameters()
{
    GLfloat ambient_color[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat diffuse_color[] = { 0.5, 0.5, 0.5, 1.0 };

    if(!getIsEnabled())
    {   
        diffuse_color[0] = diffuse_color[1] = diffuse_color[2] = 0.0;
    }   

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);
}

