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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H 1

#include <QObject>
#include <QStringList>

class Player;

class ResourceManager : public QObject
{
    Q_OBJECT

    public:
        ResourceManager(QObject *parent = 0);
        ~ResourceManager();

        void registerResource(const QString name);
        const QStringList& getResources() { return resources; }
        QString getResourceIconFilename(QString name);
        void clear();

        const QMap<QString, int> getPlayerResources(Player *player);

    protected:
        QStringList resources;
};

#endif

