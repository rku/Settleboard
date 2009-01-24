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

#ifndef OPENGLDRAWOBJECT_H
#define OPENGLDRAWOBJECT_H

#include <QtOpenGL>

class OpenGLDrawObject
{
    public:
        OpenGLDrawObject();
        ~OpenGLDrawObject();

        void render();

        void setPosX(GLfloat i) { posX = i; }
        GLfloat getPosX() { return posX; }
        void setPosY(GLfloat i) { posY = i; }
        GLfloat getPosY() { return posY; }
        void setPosZ(GLfloat i) { posZ = i; }
        GLfloat getPosZ() { return posZ; }
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

        void rotateX(GLfloat i) { setAngleX(angleX + i); }
        void rotateY(GLfloat i) { setAngleY(angleY + i); }
        void rotateZ(GLfloat i) { setAngleZ(angleZ + i); }

        void moveX(GLfloat i) { setPosX(posX + i); }
        void moveY(GLfloat i) { setPosY(posY + i); }
        void moveZ(GLfloat i) { setPosZ(posZ + i); }
        void move(GLfloat x, GLfloat y, GLfloat z)
            { moveX(x); moveY(y); moveZ(z); }

    protected:
        GLuint displayListID;

        void transform();

    private:
        GLfloat posX;
        GLfloat posY;
        GLfloat posZ;
        GLfloat scale;
        GLfloat angleX;
        GLfloat angleY;
        GLfloat angleZ;
};

#endif /* OPENGLDRAWOBJECT_H */
