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
#include "PlayerObject.h"
#include "GLGameModelProxy.h"

GLGameModelProxy::GLGameModelProxy(QObject *parent) : GLGameModel(parent)
{
    playerObject = NULL;
}

GLGameModelProxy::~GLGameModelProxy()
{
}

void GLGameModelProxy::placePlayerObject(PlayerObject *po)
{
    playerObject = po;

    if(playerObject != NULL) playerObject->setBaseObject(this);
}

bool GLGameModelProxy::getIsSelectable()
{
    // if a player object is placed, function as a proxy
    if(getIsPlayerObjectPlaced())
    {
        return playerObject->getIsSelectable();
    }

    return GLGameModel::getIsSelectable();
}

void GLGameModelProxy::setIsSelectable(bool b)
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->setIsSelectable(b);
        return;
    }

    GLGameModel::setIsSelectable(b);
}

void GLGameModelProxy::setIsHighlighted(bool b)
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->setIsHighlighted(b);
        return;
    }

    GLGameModel::setIsHighlighted(b);
}

void GLGameModelProxy::setIsEnabled(bool b)
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->setIsEnabled(b);
        return;
    }

    GLGameModel::setIsEnabled(b);
}

