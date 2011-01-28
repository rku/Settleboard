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

        QVector<Vertex3f>& getVertices() { return vertices; }
        QVector<Vertex3f>& getVertexNormals() { return vertexNormals; }
        QVector<Vertex2f>& getTextureCoords() { return textureCoords; }
        QList<GLModelFace>& getGLModelFaces() { return glModelFaces; }

        void setPosX(GLfloat i) { posX = i; }
        GLfloat getPosX() { return posX; }
        void setPosY(GLfloat i) { posY = i; }
        GLfloat getPosY() { return posY; }
        void setPosZ(GLfloat i) { posZ = i; }
        GLfloat getPosZ() { return posZ; }
        void setPos(Vertex3f v) { setPos(v.x, v.y, v.z); }
        void setPos(GLfloat x, GLfloat y, GLfloat z)
            { setPosX(x); setPosY(y); setPosZ(z); }

        void setScale(GLfloat i) { scale = i; }
        GLfloat getScale() { return scale; }

        void setAngleX(GLfloat);
        GLfloat getAngleX() { return angleX; }
        void setAngleY(GLfloat);
        GLfloat getAngleY() { return angleY; }
        void setAngleZ(GLfloat);
        GLfloat getAngleZ() { return angleZ; }

        QColor getColor() { return color; }
        void setColor(QColor c) { color = c; }

        void rotateX(GLfloat i) { setAngleX(angleX + i); }
        void rotateY(GLfloat i) { setAngleY(angleY + i); }
        void rotateZ(GLfloat i) { setAngleZ(angleZ + i); }

        void moveX(GLfloat i) { setPosX(posX + i); }
        void moveY(GLfloat i) { setPosY(posY + i); }
        void moveZ(GLfloat i) { setPosZ(posZ + i); }
        void move(GLfloat x, GLfloat y, GLfloat z)
            { moveX(x); moveY(y); moveZ(z); }

        GLfloat getWidth();
        GLfloat getHeight();
        GLfloat getDepth();

        bool getVertexGroupWithName(QString,VertexGroup&);
        QList<Vertex3f> getVerticesOfGroupWithName(QString);

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
        QVector<Vertex3f> vertices;
        QVector<Vertex3f> vertexNormals;
        QList<VertexGroup> vertexGroups;
        QVector<Vertex2f> textureCoords;
        QList<GLModelFace> glModelFaces;
        GLfloat posX;
        GLfloat posY;
        GLfloat posZ;
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
