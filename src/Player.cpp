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

#include <time.h>

#include "Game.h"
#include "PlayerObject.h"
#include "Player.h"

Player::Player(QObject *parent) : QObject(parent)
{
    isDisconnected = false;
    isLocal = false;
    isSpectator = false;
    isReady = false;
    color = Qt::red;
    id = QUuid::createUuid();
    winningPoints = 0;

    qsrand(time(NULL));
    name = QString("Player%1").arg(qrand());
}

Player::~Player()
{
    while(!objects.isEmpty()) delete objects.take(objects.keys()[0]);
}

Player* Player::findPlayerWithId(QString theId)
{
    Player *player = NULL;

    // find player
    QList<Player*> players = GAME->getPlayers();
    QList<Player*>::iterator i;
    for(i = players.begin(); i != players.end(); ++i)
    {
        Player *p = *i;
        if(p->getId() == theId)
        {
            player = *i;
            break;
        }
    }

    return player;
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

    qDebug() << "Unplaced object requested but all are placed!";
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
    stream << obj.object->getId();
    stream << obj.object->getName();
    stream << obj.object->getColor();
    stream << obj.object->getIsReady();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, PlayerPtr &obj)
{
    QString id, name;
    QColor color;
    bool isReady;

    stream >> id;
    stream >> name;
    stream >> color;
    stream >> isReady;

    obj.object = NULL;
    Q_ASSERT(!name.isEmpty());

    // find player
    obj.object = Player::findPlayerWithId(id);

    // create new one
    if(obj.object == NULL)
    {
        obj.object = new Player();
        obj.object->setId(id);
        obj.object->setName(name);
        obj.object->setColor(color);
        obj.object->setIsReady(isReady);
    }

    return stream;
}

