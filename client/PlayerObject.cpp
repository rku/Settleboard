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

PlayerObject::PlayerObject(Player *_owner, QString _type, QObject *parent)
    : GLGameModel(parent), type(_type), owner(_owner)
{
    load(FileManager::getPathOfGLObject(type));
    setColor(owner->getColor());

    baseObject = NULL;
}

// QDataStream operators

QDataStream &operator<<(QDataStream &stream, const PlayerObjectPtr &obj)
{
    stream << obj.object->getPos();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, PlayerObjectPtr &obj)
{
    QVector3D v;

    stream >> v;
    // find object with vertex v
    // obj.object = Game::getInstance()->getBoard()->getPlayerObjectAtVertex(v);

    return stream;
}

