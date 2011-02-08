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
#include "PlayerObject.h"
#include "Player.h"

Player::Player(QObject *parent) : QObject(parent)
{
    isSpectator = false;
    color = Qt::red;

    qsrand(time(NULL));
    name = QString("Player%1").arg(qrand());
}

Player::~Player()
{
    while(!objects.isEmpty()) delete objects.take(objects.keys()[0]);
}

void Player::addObjectOfType(QString type)
{
    PlayerObject *newObj = new PlayerObject(this, type, this);

    Q_ASSERT(newObj != NULL);
    objects.insertMulti(type, newObj);

    qDebug() << "Added" << type << "object to player" << name;
}

uint Player::getNumberOfObjectsOfType(QString type)
{
    Q_ASSERT(objects.contains(type));
    return objects.count(type);
}

uint Player::getNumberOfPlacedObjectsOfType(QString type)
{
    uint num = 0;

    Q_ASSERT(objects.contains(type));
    QList<PlayerObject*> objs = objects.values(type);

    for(int i = 0; i < objs.size(); i++)
    {
        if(objs.at(i)->getIsPlaced()) num++;
    }

    return num;
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

// QDataStream operators

QDataStream &operator<<(QDataStream &stream, const PlayerPtr &obj)
{
    stream << obj.object->getName();
    stream << obj.object->getColor();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, PlayerPtr &obj)
{
    QString name;
    QColor color;

    stream >> name;
    stream >> color;

    obj.object = NULL;
    Q_ASSERT(!name.isEmpty());

    // find player
    QList<Player*> players = GAME->getPlayers();
    QList<Player*>::iterator i;
    for(i = players.begin(); i != players.end(); ++i)
    {
        Player *p = *i;
        if(p->getName() == name && p->getColor() == color)
        {
            obj.object = *i;
            break;
        }
    }

    Q_ASSERT(obj.object != NULL);
    return stream;
}

