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

#include "GLObject.h"

GLObject::GLObject()
{
    displayListID = glGenLists(1);

    posX   = posY 
           = posZ 
           = 0.0f;
    scale  = 1.0f;
    angleX = 0.0f;
    angleY = 0.0f;
    angleZ = 0.0f;
}

GLObject::~GLObject()
{
    glDeleteLists(displayListID, 1);
}

void GLObject::render()
{
    glPushMatrix();
    transform();
    glCallList(displayListID);
    glPopMatrix();
}

void GLObject::setAngleX(GLfloat i)
{
    angleX = (i > 0.0f) ? i : 0.0f;
    if(angleX > 360.0f) angleX -= 360.0f;
}

void GLObject::setAngleY(GLfloat i)
{
    angleY = (i > 0.0f) ? i : 0.0f;
    if(angleY > 360.0f) angleX -= 360.0f;
}

void GLObject::setAngleZ(GLfloat i)
{
    angleZ = (i > 0.0f) ? i : 0.0f;
    if(angleZ > 360.0f) angleZ -= 360.0f;
}

void GLObject::transform()
{
    glTranslatef(posX, posY, posZ);

    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);

    glScalef(scale, scale, scale);
}

