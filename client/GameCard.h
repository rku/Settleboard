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

#ifndef GAMECARD_H
#define GAMECARD_H 1

#include <QString>
#include <QtDebug>

#include "GamePixmap.h"

class GameCard
{
    public:
        GameCard(const GameCard &c);
        GameCard(QString _type,
                 QString _name,
                 QString _description,
                 QString _pixmapFilename = QString(),
                 QString _playRule = QString(),
                 QString _canPlayRule = QString(),
                 bool _isSecret = true);

        const QString &getType() { return type; }
        const QString &getName() { return name; }
        const QString &getDescription() { return description; }
        const GamePixmap &getPixmap() { return pixmap; }
        const QString &getPlayRule() { return playRule; }
        const QString &getCanPlayRule() { return canPlayRule; }
        bool getIsSecret() { return isSecret; }

        static GamePixmap &getCoverPixmap();
        static GameCard createResourceCardWheat();
        static GameCard createResourceCardOre();
        static GameCard createResourceCardLumber();
        static GameCard createResourceCardWool();
        static GameCard createResourceCardClay();
        static GameCard createDevelopmentCardKnight();
        static GameCard createDevelopmentCardBuildRoad();
        static GameCard createDevelopmentCardMonopoly();
        static GameCard createDevelopmentCardInvention();
        static GameCard createDevelopmentCardWinningPoint();

    private:
        QString type;
        QString name;
        QString description;
        GamePixmap pixmap;
        QString playRule;
        QString canPlayRule;
        bool isSecret;
};

#endif

