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

#ifndef GLGAMEMODEL_H
#define GLGAMEMODEL_H

#include <QtOpenGL>
#include <QVector>

#include "GameObject.h"
#include "GLTypes.h"

class OBJGLLoader;
class Game;

class GLGameModel : public GameObject
{
    public:
        GLGameModel(Game*);
        ~GLGameModel();

        void load(QString filename, QColor objColor = Qt::white);
        virtual void draw();

        void setName(QString s) { name = s; }
        QString getName() { return name; }

        QVector<QVector3D>& getVertices() { return vertices; }
        QVector<QVector3D>& getVertexNormals() { return vertexNormals; }
        QVector<QVector2D>& getTextureCoords() { return textureCoords; }
        QList<GLModelFace>& getGLModelFaces() { return glModelFaces; }

        void setPosX(GLfloat i) { pos.setX(i); }
        GLfloat getPosX() { return pos.x(); }
        void setPosY(GLfloat i) { pos.setY(i); }
        GLfloat getPosY() { return pos.y(); }
        void setPosZ(GLfloat i) { pos.setZ(i); }
        GLfloat getPosZ() { return pos.z(); }
        void setPos(QVector3D v) { pos = v; }
        void setPos(GLfloat x, GLfloat y, GLfloat z)
            { pos = QVector3D(x, y, z); }

        void setScale(GLfloat i) { scale = i; }
        GLfloat getScale() { return scale; }

        void setAngleX(GLfloat);
        GLfloat getAngleX() { return angleX; }
        void setAngleY(GLfloat);
        GLfloat getAngleY() { return angleY; }
        void setAngleZ(GLfloat);
        GLfloat getAngleZ() { return angleZ; }

        void pointToVertex(QVector3D);
        void pointInDirection(QVector3D);

        QColor getColor() { return color; }
        void setColor(QColor c) { color = c; }

        void rotateX(GLfloat i) { setAngleX(angleX + i); }
        void rotateY(GLfloat i) { setAngleY(angleY + i); }
        void rotateZ(GLfloat i) { setAngleZ(angleZ + i); }

        void moveX(GLfloat i) { setPosX(pos.x() + i); }
        void moveY(GLfloat i) { setPosY(pos.y() + i); }
        void moveZ(GLfloat i) { setPosZ(pos.z() + i); }
        void move(GLfloat x, GLfloat y, GLfloat z)
            { moveX(x); moveY(y); moveZ(z); }

        GLfloat getWidth();
        GLfloat getHeight();
        GLfloat getDepth();

        bool getVertexGroupWithName(QString,VertexGroup&);
        QList<QVector3D> getVerticesOfGroupWithName(QString);

        void setIsLightingEnabled(bool b) { isLightingEnabled = b; }
        bool getIsLightingEnabled() { return isLightingEnabled; }
        void setIsHighlighted(bool b) { isHighlighted = b; }
        bool getIsHighlighted() { return isHighlighted; }
        void setIsSelectable(bool b) { isSelectable = b; }
        bool getIsSelectable() { return isSelectable; }
        void setIsVisible(bool b) { isVisible = b; }
        bool getIsVisible() { return isVisible; }
        void setIsEnabled(bool b) { isEnabled = b; }
        bool getIsEnabled() { return isEnabled; }

    protected:
        void create();
        void createBorder();
        void transform();

        GLuint displayListID;
        GLuint borderDisplayListID;
        bool created;
        QString name;
        QVector<QVector3D> vertices;
        QVector<QVector3D> vertexNormals;
        QList<VertexGroup> vertexGroups;
        QVector<QVector2D> textureCoords;
        QList<GLModelFace> glModelFaces;
        QVector3D pos;
        GLfloat scale;
        GLfloat angleX;
        GLfloat angleY;
        GLfloat angleZ;
        QColor color;
        bool isLightingEnabled;
        bool isHighlighted;
        bool isSelectable;
        bool isVisible;
        bool isEnabled;
};

#endif /* GLGAMEMODEL_H */
