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

Player::~Player()
{
    while(!objects.isEmpty()) delete objects.take(objects.keys()[0]);
}

void Player::addObjectOfType(QString type)
{
    PlayerObject *newObj = new PlayerObject(game, this, type);

    Q_ASSERT(newObj != NULL);
    objects.insertMulti(type, newObj);

    qDebug() << "Added" << type << "object to player" << name;
}

uint Player::getNumberOfObjectsOfType(QString type)
{
    Q_ASSERT(objects.contains(type));
    return objects.count(type);
}

uint Player::getNumberOfUnplacedObjectsOfType(QString type)
{
    uint num = 0;

    Q_ASSERT(objects.contains(type));
    QList<PlayerObject*> objs = objects.values(type);

    for(int i = 0; i < objs.size(); i++)
    {
        if(!objs.at(i)->getIsPlaced()) num++;
    }

    return num;
}

PlayerObject *Player::getUnplacedObjectOfType(QString type)
{
    Q_ASSERT(objects.contains(type));
    QList<PlayerObject*> objs = objects.values(type);

    for(int i = 0; i < objs.size(); i++)
    {
        if(!objs.at(i)->getIsPlaced()) return objs.at(i);
    }

    return NULL;
}

const QList<PlayerObject*> Player::getObjectsOfType(QString type)
{
    Q_ASSERT(objects.contains(type));
    return objects.values(type);
}

