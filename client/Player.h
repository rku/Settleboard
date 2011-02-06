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

#include "GameCardStack.h"

class Game;
class PlayerObject;

class Player : public QObject
{
    Q_OBJECT

    public:
        Player(QTcpSocket* = NULL, QObject *parent = 0);
        ~Player();

        const QColor &getColor() { return color; }
        void setColor(QColor c) { color = c; }
        const QString &getName() { return name; }
        void setName(QString n) { name = n; }
        QTcpSocket* getSocket() { return socket; }
        void setSocket(QTcpSocket *s) { socket = s; }
        bool getIsLocal() { return (socket == NULL); }
        bool getIsSpectator() { return isSpectator; }
        void setIsSpectator(bool b) { isSpectator = b; }

        GameCardStack *getCardStack() { return &cards; }

        void addObjectOfType(QString type);
        uint getNumberOfObjectsOfType(QString type);
        uint getNumberOfPlacedObjectsOfType(QString type);
        uint getNumberOfUnplacedObjectsOfType(QString type);
        PlayerObject *getUnplacedObjectOfType(QString type);
        const QList<PlayerObject*> getObjectsOfType(QString type);
        const QMap<QString, PlayerObject*> &getObjects() { return objects; }

    protected:
        bool isSpectator;
        QTcpSocket *socket;
        QColor color;
        QString name;
        GameCardStack cards;
        QMap<QString, PlayerObject*> objects;
};

Q_DECLARE_METATYPE(Player*);

#endif

