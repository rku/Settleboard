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

#include <QColor>

#include <QColor>

#include "GLGameModel.h"
#include "GLWidget.h"
#include "Game.h"
#include "PlayerObject.h"
#include "OBJGLLoader.h"

GLGameModel::GLGameModel(QObject *parent) : QObject(parent)
{
    created = false;

    displayListID = glGenLists(1);
    frameDisplayListID = glGenLists(1);

    pos    = QVector3D(0.0, 0.0, 0.0); 
    scale  = 1.0f;
    angleX = 0.0f;
    angleY = 0.0f;
    angleZ = 0.0f;

    color = Qt::gray;
    setIsLightingEnabled(true);
    setIsHighlighted(false);
    setIsSelectable(false);
    setIsVisible(true);
    setIsEnabled(true);
}

GLGameModel::~GLGameModel()
{
    glDeleteLists(displayListID, 1);
    glDeleteLists(frameDisplayListID, 1);
}

void GLGameModel::create()
{
    GLuint currentTex = 65535;
    TextureManager *tm = GAME->getTextureManager();

    glNewList(displayListID, GL_COMPILE);

    if(getIsLightingEnabled()) glEnable(GL_LIGHTING);

    for(int i = 0; i < glModelFaces.size(); ++i)
    {
        GLModelFace face = glModelFaces.at(i);

        // set or unset texture if neccessary
        if(!face.materialName.isEmpty() &&
            materials.contains(face.materialName))
        {
            QString textureFileName = materials.value(face.materialName);
            GLuint texId = tm->getTextureId(textureFileName);
            if(currentTex != texId)
            {
                glDisable(GL_COLOR_MATERIAL);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texId);
                GAME->getGLWidget()->qglColor(Qt::white);
                currentTex = texId;
            }
        }
        else
        {
            if(currentTex != 0)
            {
                glEnable(GL_COLOR_MATERIAL);
                currentTex = 0;
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_TEXTURE_2D);
            }
        }

        // draw the model

        glBegin(GL_POLYGON);

        for(int n = 0; n < face.vertexIds.size(); ++n)
        {
            int x = face.vertexIds.at(n);

            if(x < 0 || x >= vertices.size()) continue;

            QVector3D v = vertices.at(x);

            if(n < face.textureCoordIds.size() && currentTex > 0)
            {
                x = face.textureCoordIds.at(n);
                QVector2D vt = textureCoords.at(x);
                glTexCoord2f(vt.x(), vt.y());
            } 

            if(n < face.vertexNormalIds.size())
            {
                x = face.vertexNormalIds.at(n);
                QVector3D vn = vertexNormals.at(x);
                glNormal3f(vn.x(), vn.y(), vn.z());
            }

            glVertex3f(v.x(), v.y(), v.z());
        }

        glEnd();
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEndList();

    createFrame();

    created = true;
}

void GLGameModel::createFrame()
{
    glNewList(frameDisplayListID, GL_COMPILE);

    for(int i = 0; i < glModelFaces.size(); ++i)
    {
        GLModelFace face = glModelFaces.at(i);

        // draw the model
        glBegin(GL_POLYGON);

        for(int n = 0; n < face.vertexIds.size(); ++n)
        {
            int x = face.vertexIds.at(n);

            if(x < 0 || x >= vertices.size()) continue;

            QVector3D v = vertices.at(x);
            glVertex3f(v.x(), v.y(), v.z());
        }

        glEnd();
    }

    glEndList();
}

void GLGameModel::load(QString filename, QColor objColor)
{
    OBJGLLoader *loader = GAME->getOBJGLLoader();
    OBJ *obj = loader->load(filename);

    vertices      = obj->vertices;
    vertexNormals = obj->vertexNormals;
    vertexGroups  = obj->vertexGroups;
    textureCoords = obj->textureCoords;
    glModelFaces  = obj->glModelFaces;

    setColor(objColor);
}

void GLGameModel::setupLightingParameters()
{
    if(getIsEnabled())
    {
        GLfloat ambient_color[] = { 0.6, 0.6, 0.6, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
        GLfloat diffuse_color[] = { 0.3, 0.3, 0.3, 1.0 };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);
    }
    else
    {
        GLfloat ambient_color[] = { 0.2, 0.2, 0.2, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
        GLfloat diffuse_color[] = { 0.0, 0.0, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);
        glEnable(GL_LIGHTING);
    }
}

void GLGameModel::highlight()
{
    GLfloat light_position[] = { (float)pos.x(), (float)pos.y() + 6.0, (float)pos.z() };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_specular);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void GLGameModel::draw()
{
    if(!created) create();

    glPushMatrix();
    transform();
    GAME->getGLWidget()->qglColor(color);
    if(getIsHighlighted()) highlight();
    setupLightingParameters();
    glCallList(displayListID);
    glDisable(GL_LIGHT1);
    glFinish();
    glPopMatrix();
}

void GLGameModel::drawOutline(QColor outlineColor)
{
    if(!created) create();

    QColor preservedColor = color;
    color = outlineColor;

    glPushMatrix();
    transform();
    GAME->getGLWidget()->qglColor(color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCallList(frameDisplayListID);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glFinish();
    glPopMatrix();

    color = preservedColor;
}

void GLGameModel::setAngleX(GLfloat i)
{
    angleX = (i > 0.0f) ? i : 0.0f;
    while(angleX > 360.0f) angleX -= 360.0f;
}

void GLGameModel::setAngleY(GLfloat i)
{
    angleY = (i > 0.0f) ? i : 0.0f;
    while(angleY > 360.0f) angleX -= 360.0f;
}

void GLGameModel::setAngleZ(GLfloat i)
{
    angleZ = (i > 0.0f) ? i : 0.0f;
    while(angleZ > 360.0f) angleZ -= 360.0f;
}

void GLGameModel::pointToVertex(QVector3D vertex)
{
    // rotate the object to point to vertex
    pointInDirection( vertex - pos );
}

void GLGameModel::pointInDirection(QVector3D d)
{
    if(d.isNull())
    {
        setAngleY(0.0);
        return;
    }

    // roate the object to point in direction
    // we only rotate in the x-z plane
    qreal cos_y = d.normalized().z();
    setAngleY(360.0 * qAcos(cos_y) / (2*M_PI));

    //qDebug() << "pointing to" << d << "with angles" << angleX << angleY << angleZ;
}

void GLGameModel::transform()
{
    glTranslatef(pos.x(), pos.y(), pos.z());

    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);

    glScalef(scale, scale, scale);
}

GLfloat GLGameModel::getWidth()
{
    qreal xMin = 0, xMax = 0;

    for(int i = 0; i < vertices.size(); ++i)
    {
        xMin = qMin(xMin, vertices.at(i).x());
        xMax = qMax(xMax, vertices.at(i).x());
    }

    return xMax - xMin;
}

GLfloat GLGameModel::getHeight()
{
    qreal yMin = 0, yMax = 0;

    for(int i = 0; i < vertices.size(); ++i)
    {   
        yMin = qMin(yMin, vertices.at(i).y());
        yMax = qMax(yMax, vertices.at(i).y());
    }   

    return yMax - yMin;
}

GLfloat GLGameModel::getDepth()
{
    float zMin = 0, zMax = 0;

    for(int i = 0; i < vertices.size(); ++i)
    {   
        zMin = qMin(zMin, (float)vertices.at(i).z());
        zMax = qMax(zMax, (float)vertices.at(i).z());
    }   

    return zMax - zMin;
}

bool GLGameModel::getVertexGroupWithName(QString name, VertexGroup &group)
{
    for(int i = 0; i < vertexGroups.size(); i++)
    {
        if(vertexGroups.at(i).name == name)
        {
            group = vertexGroups.at(i);
            return true;
        }
    }

    qDebug() << "Unknown vertex group: " << name;
    return false;
}

QList<QVector3D> GLGameModel::getVerticesOfGroupWithName(QString name)
{
    VertexGroup group;
    QList<QVector3D> list;

    if(!getVertexGroupWithName(name, group)) return list;

    for(int i = 0; i < group.indices.size(); i++)
    {
        if(group.indices.at(i) >= (unsigned int)vertices.size()) continue;
        list.append(vertices.at(group.indices.at(i)));
    }

    return list;
}

void GLGameModel::setTexture(const QString &materialName, const QString &fileName)
{
    materials.insert(materialName, fileName);
}

