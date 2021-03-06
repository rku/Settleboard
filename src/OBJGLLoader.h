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

// this obj and mtl file loader implements only the features
// we need at the moment - it is far away from being a full featured
// obj file loader

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QtOpenGL>
#include <QString>
#include <QList>
#include <QFile>

#include "GLTypes.h"

typedef struct _OBJ {
    QString name;
    QVector<QVector3D> vertices;
    QVector<QVector3D> vertexNormals;
    QList<VertexGroup> vertexGroups;
    QVector<QVector2D> textureCoords;
    QList<GLModelFace> glModelFaces;
} OBJ;

class GLModel;
class Game;

class OBJGLLoader
{
    public:
        OBJGLLoader();
        ~OBJGLLoader();

        OBJ* load(QString filename);
        int getOBJCacheId(QString filename);
        OBJ& getOBJByCacheId(int id);

    private:
        QList<OBJ> objCache;
};

#endif

