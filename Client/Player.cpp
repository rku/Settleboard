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

#include "Player.h"
#include "Resource.h"
#include "PlayerObject.h"

Player::Player(Game *_game)
    : GameObject(_game)
{
    color = Qt::red;
    name = "unknown";
    setIsLocal(true);
}

QList<Resource*>& Player::getResources()
{
    return resources;
}

QList<PlayerObject*>& Player::getPlayerObjects()
{
    return objects;
}
