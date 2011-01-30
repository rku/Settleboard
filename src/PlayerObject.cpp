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

#include "Game.h"
#include "Player.h"
#include "GLGameModelProxy.h"
#include "FileManager.h"
#include "PlayerObject.h"

PlayerObject::PlayerObject(Game *_game, Player *_player, QString _type)
    : GLGameModel(_game), type(_type), player(_player)
{
    load(FileManager::getPathOfResource("Objects", type, "obj"));
    setColor(player->getColor());

    baseObject = NULL;
}

