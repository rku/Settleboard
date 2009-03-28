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

#ifndef GLTYPES_H
#define GLTYPES_H

#include <QtOpenGL>

typedef struct _Vertex3f {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vertex3f;

typedef struct _Vertex2f {
    GLfloat x;
    GLfloat y;
} Vertex2f;

typedef struct _GLModelFace {
    QString texFilename;
    QVector<GLuint> vertexIds;
    QVector<GLuint> vertexNormalIds;
    QVector<GLuint> textureCoordIds; 
} GLModelFace;

typedef struct _VertexGroup {
    QString name;
    QList<GLuint> indices;
} VertexGroup;

#endif /* GLTYPES_H */
