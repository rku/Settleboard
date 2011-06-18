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

#include "FileManager.h"
#include "Player.h"
#include "GameCardStack.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(QObject *parent)
    : QObject(parent)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::registerResource(const QString name)
{
    if(!resources.contains(name))
    {
        resources.append(name);
        qDebug() << "Registered new resource:" << name;
    }
}

QString ResourceManager::getResourceIconFilename(QString name)
{
    return FileManager::getPathOfImage(QString("%1.png").arg(name));
}

const QMap<QString, int> ResourceManager::getPlayerResources(Player *player)
{
    QMap<QString, int> playerResources;
    GameCardStack *stack = player->getCardStack();

    foreach(QString name, resources)
    {
        playerResources.insert(name,stack->getNumberOfCards("Resource", name));
    }

    return playerResources;
}

void ResourceManager::clear()
{
    resources.clear();
}

