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

#include "GLTypes.h"

class OBJGLLoader;
class Game;

class GLGameModel : public QObject
{
    Q_OBJECT

    public:
        GLGameModel(QObject *parent = 0);
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
        virtual const QVector3D& getPos() { return pos; }
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

        virtual void setIsLightingEnabled(bool b) { isLightingEnabled = b; }
        virtual bool getIsLightingEnabled() { return isLightingEnabled; }
        virtual void setIsHighlighted(bool b) { isHighlighted = b; }
        virtual bool getIsHighlighted() { return isHighlighted; }
        virtual void setIsSelectable(bool b) { isSelectable = b; }
        virtual bool getIsSelectable() { return isSelectable; }
        virtual void setIsVisible(bool b) { isVisible = b; }
        virtual bool getIsVisible() { return isVisible; }
        virtual void setIsEnabled(bool b) { isEnabled = b; }
        virtual bool getIsEnabled() { return isEnabled; }

    protected:
        void create();
        void createBorder();
        void transform();
        void highlight();
        virtual void setupLightingParameters();

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

Q_DECLARE_METATYPE(GLGameModel*);

class GLGameModelPtr
{
    public:
        GLGameModelPtr() : object(NULL) {}
        GLGameModelPtr(GLGameModel *p) { object = p; }
        GLGameModelPtr(const GLGameModelPtr &c) { object = c.object; }
        GLGameModel *getObject() { return object; }

        friend QDataStream &operator<<(QDataStream&, const GLGameModelPtr&);
        friend QDataStream &operator>>(QDataStream&, GLGameModelPtr&);

        GLGameModel *object;
};

Q_DECLARE_METATYPE(GLGameModelPtr);

#endif /* GLGAMEMODEL_H */
