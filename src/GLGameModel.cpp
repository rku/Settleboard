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
    GLuint currentTex = -1;
    TextureManager *tm = game->getTextureManager();

    glNewList(displayListID, GL_COMPILE);

    for(int i = 0; i < glModelFaces.size(); ++i)
    {
        GLModelFace face = glModelFaces.at(i);

        // set or unset texture if neccessary
        if(!face.texFilename.isEmpty())
        {
            GLuint texId = tm->getTextureId(face.texFilename);
            if(currentTex != texId)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texId);
                currentTex = texId;
            }
        }
        else
        {
            if(currentTex != 0)
            {
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

            Vertex3f v = vertices.at(x);

            if(n < face.textureCoordIds.size() && currentTex > 0)
            {
                x = face.textureCoordIds.at(n);
                Vertex2f vt = textureCoords.at(x);
                glTexCoord2f(vt.x, vt.y);
            } 

            if(n < face.vertexNormalIds.size())
            {
                x = face.vertexNormalIds.at(n);
                Vertex3f vn = vertexNormals.at(x);
                glNormal3f(vn.x, vn.y, vn.z);
            }

            glVertex3f(v.x, v.y, v.z);
        }

        glEnd();
    }

    glEndList();

    created = true;
}

void GLGameModel::load(QString filename)
{
    OBJGLLoader *loader = game->getOBJGLLoader();
    OBJ *obj = loader->load(filename);

    vertices      = obj->vertices;
    vertexNormals = obj->vertexNormals;
    textureCoords = obj->textureCoords;
    glModelFaces  = obj->glModelFaces;
}

void GLGameModel::draw()
{
    if(!created) create();

    glPushMatrix();
    transform();
    glCallList(displayListID);
    glFinish();
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

