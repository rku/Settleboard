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

#ifndef MAP_H
#define MAP_H 1

#include <QObject>
#include <QList>
#include "HexTile.h"

typedef QList<HexTile*> HexTileList;

class Map : public QObject
{
    Q_OBJECT

    public:
        Map(QObject *parent = 0);
        virtual ~Map();

        virtual const HexTileList& getTiles() { return tiles; }
        virtual void shuffleTiles() {}
        virtual void shuffleNumberChips() {}

        const QSize& getSize() { return size; }
        unsigned int getWidth() { return size.width(); }
        unsigned int getHeight() { return size.height(); }
        const QString& getAuthor() { return author; }
        const QString& getTitle() { return title; }
        const QString& getDescription() { return description; }

    protected:
        void createTile(
            unsigned int x, unsigned int y,
            HexTile::HexTileType, bool fixed = true,
            int chipNumber = -1,
            HexTile::HexTileType portType = HexTile::NoType);
        void setSize(const QSize& s) { size = s; }
        void setAuthor(const QString &s) { author = s; }
        void setTitle(const QString &s) { title = s; }
        void setDescription(const QString &s) { description = s; }

        HexTileList tiles;
        QSize size;
        QString author;
        QString title;
        QString description;
};

#endif

