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

#include "GLGameModel.h"
#include "Game.h"
#include "OBJGLLoader.h"

GLGameModel::GLGameModel(Game *_game)
    : game(_game)
{
    created = false;

    displayListID = glGenLists(1);

    posX   = posY 
           = posZ 
           = 0.0f;
    scale  = 1.0f;
    angleX = 0.0f;
    angleY = 0.0f;
    angleZ = 0.0f;
}

GLGameModel::~GLGameModel()
{
    glDeleteLists(displayListID, 1);
}

void GLGameModel::create()
{
    TextureManager *tm = game->getTextureManager();

    glNewList(displayListID, GL_COMPILE);

    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoords.data());

    for(int i = 0; i < glModelFaces.size(); ++i)
    {
        GLModelFace face = glModelFaces.at(i);
        GLuint num = face.vertexIDs.size();

        if(!face.texFilename.isEmpty())
        {
            GLuint texId = tm->getTextureId(face.texFilename);
            glBindTexture(GL_TEXTURE_2D, texId);
        }

        glDrawElements(GL_POLYGON, num,
            GL_UNSIGNED_INT, face.vertexIDs.data());
    }

    glEndList();

    created = true;
}

void GLGameModel::load(QString filename)
{
    OBJGLLoader *loader = game->getOBJGLLoader();
    int id = loader->load(filename);
    OBJ obj = loader->getOBJByCacheID(id);

    vertices = obj.vertices;
    vertexNormals = obj.vertexNormals;
    textureCoords = obj.textureCoords;
    glModelFaces = obj.glModelFaces;
}

void GLGameModel::draw()
{
    if(!created) create();

    glPushMatrix();
    transform();
    glCallList(displayListID);
    glPopMatrix();
}

void GLGameModel::setAngleX(GLfloat i)
{
    angleX = (i > 0.0f) ? i : 0.0f;
    if(angleX > 360.0f) angleX -= 360.0f;
}

void GLGameModel::setAngleY(GLfloat i)
{
    angleY = (i > 0.0f) ? i : 0.0f;
    if(angleY > 360.0f) angleX -= 360.0f;
}

void GLGameModel::setAngleZ(GLfloat i)
{
    angleZ = (i > 0.0f) ? i : 0.0f;
    if(angleZ > 360.0f) angleZ -= 360.0f;
}

void GLGameModel::transform()
{
    glTranslatef(posX, posY, posZ);

    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);

    glScalef(scale, scale, scale);
}

