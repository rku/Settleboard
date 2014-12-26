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

const QString FileManager::getPathOfImage(const QString &filename)
{
    return getPathOfResource("Images", filename);
}

const QString FileManager::getPathOfStyleSheet(const QString &name)
{
    return getPathOfResource("Styles", name, "qss");
}

const QString FileManager::getPathOfMap(const QString &name)
{
    return getPathOfResource("Maps", name, "rsm");
}

const QString FileManager::getPathOfTexture(const QString &name)
{
    return getPathOfResource("Textures", name, "obj");
}

const QString FileManager::getPathOfGLObject(const QString &name)
{
    return getPathOfResource("Objects", name, "obj");
}

const QString FileManager::getPathOfResource(const QString &group,
        const QString &name, const QString &type)
{
    QString filename("%1.%2");
    return FileManager::getPathOfResource(group, filename.arg(name, type));
}

const QString FileManager::getPathOfResource(const QString &group,
    const QString &filename)
{
    QString path("%1/%2/%3");
    return path.arg(FileManager::getResourceRootPath(), group, filename);
}

