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

// this obj and mtl file loader implements only the features
// we need at the moment - it is far away from being a full featured
// obj file loader

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QtOpenGL>
#include <QString>
#include <QList>
#include <QFile>

typedef struct _Vertex {
    _Vertex() { x = 0; y = 0; z = 0; };
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vertex;

typedef struct _FaceData {
    _FaceData() { vertexId = textureVertexId = vertexNormalId = -1; };
    int vertexId;
    int textureVertexId;
    int vertexNormalId;
} FaceData;

typedef struct _Face {
    QList<FaceData> data;
    QString material;
} Face;

typedef struct _Material {
    void clear()
    {
        name = QString();
        texFilename = QString();
        ka[0] = ka[1] = ka[2] = 0; ka[3] = 1.0;
        kd[0] = kd[1] = kd[2] = 0; kd[3] = 1.0;
        ks[0] = ks[1] = ks[2] = 0; ks[3] = 1.0;
    };
    QString name;
    GLfloat ka[4];
    GLfloat kd[4];
    GLfloat ks[4];
    QString texFilename;
} Material;

class OBJGLLoader
{
    public:
        OBJGLLoader();

        bool load(QString filename);

    private:
        void loadMaterials(QString mtlFilename);
        void setGLMaterial(QString name);
        void createGLModel();

        QList<Vertex> vertices;
        QList<Vertex> vertexNormals;
        QList<Vertex> textureCoords;
        QList<Face> faces;
        QList<Material> materials;
};

#endif

