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

#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QList>
#include <QColor>
#include <QMap>
#include <QTcpSocket>
#include <QUuid>

#include "GameCardStack.h"

class Game;
class PlayerObject;

class Player : public QObject
{
    Q_OBJECT

    public:
        Player(QObject *parent = 0);
        ~Player();

        void setId(QString _id) { id = _id; }
        const QString getId() { return id.toString(); }
        const QColor &getColor() { return color; }
        void setColor(QColor c) { color = c; }
        const QString &getName() { return name; }
        void setName(QString n) { name = n; }
        void setIsLocal(bool b) { isLocal = b; }
        bool getIsLocal() { return isLocal; }
        void setIsSpectator(bool b) { isSpectator = b; }
        bool getIsSpectator() { return isSpectator; }

        GameCardStack *getCardStack() { return &cards; }

        void addObjectOfType(QString type);
        uint getNumberOfObjectsOfType(QString type);
        uint getNumberOfPlacedObjectsOfType(QString type);
        uint getNumberOfUnplacedObjectsOfType(QString type);
        PlayerObject *getUnplacedObjectOfType(QString type);
        const QList<PlayerObject*> getObjectsOfType(QString type);
        const QMap<QString, PlayerObject*> &getObjects() { return objects; }

    protected:
        QUuid id;
        bool isLocal;
        bool isSpectator;
        QColor color;
        QString name;
        GameCardStack cards;
        QMap<QString, PlayerObject*> objects;
};

Q_DECLARE_METATYPE(Player*);

class PlayerPtr
{
    public:
        PlayerPtr() : object(NULL) {}
        PlayerPtr(Player *p) { object = p; }
        PlayerPtr(const PlayerPtr &c) { object = c.object; }
        Player *getObject() { return object; }

        friend QDataStream &operator<<(QDataStream&, const PlayerPtr&);
        friend QDataStream &operator>>(QDataStream&, PlayerPtr&);

    protected:
        Player *object;
};

Q_DECLARE_METATYPE(PlayerPtr);

#endif

