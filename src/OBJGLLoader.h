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

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QtOpenGL>
#include <QString>
#include <QList>
#include <QFile>

typedef struct _Vertex {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vertex;

typedef struct _FaceData {
    _FaceData()
    {
        vertexId        = -1;
        textureVertexId = -1;
        vertexNormalId  = -1;
    };
    int vertexId;
    int textureVertexId;
    int vertexNormalId;
} FaceData;

typedef struct _Face {
    QList<FaceData> data;
} Face;

class OBJGLLoader
{
    public:
        OBJGLLoader();

        bool load(QString filename);

    private:
        void createGLModel();

        QList<Vertex> vertices;
        QList<Face> faces;
};

#endif

