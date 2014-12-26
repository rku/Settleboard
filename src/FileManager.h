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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H 1

#include <QString>

class FileManager
{
    public:
    static const QString getPathOfImage(const QString &filename);
    static const QString getPathOfStyleSheet(const QString &name);
    static const QString getPathOfMap(const QString &name);
    static const QString getPathOfTexture(const QString &name);
    static const QString getPathOfGLObject(const QString &name);
    static const QString getPathOfResource(const QString &group,
        const QString &name, const QString &type);
    static const QString getPathOfResource(const QString &group,
        const QString &filename);
    static const QString getResourceRootPath() { return QString(":"); }
};

#endif

