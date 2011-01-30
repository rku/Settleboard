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

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "GameObject.h"

#define GL_SELBUF_SIZE      512

class Game;

class GLWidget : public QGLWidget, GameObject
{
    Q_OBJECT

    public:
        GLWidget(Game *_game, QWidget *parent = 0);
        ~GLWidget();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        void beginGLSelection(QPoint);
        QList<GLuint> endGLSelection();

    public slots:

    signals:

    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);
        void setPerspective();

    private:
        void updateCameraPos();

        GLdouble cameraAngleX;
        GLdouble cameraAngleY;
        GLdouble cameraDistance;
        GLdouble cameraMinDistance;
        GLdouble cameraMaxDistance;
        QPoint   lastMousePos;
        GLuint   selectionBuffer[GL_SELBUF_SIZE];
 };

 #endif
