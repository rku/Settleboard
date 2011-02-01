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

#include <QtGui>
#include <QtOpenGL>

#include "GLWidget.h"
#include "GameUI.h"
#include "Game.h"

#define CAM_X_ROT_DELTA         0.006
#define CAM_Y_ROT_DELTA         0.002
#define CAM_ZOOM_DELTA          1
#define CAM_MAX_Y_ANGLE         (0.49*M_PI)      // max 90 degree
#define CAM_MIN_Y_ANGLE         0.1 
#define CAM_FOV                 65.0

GLWidget::GLWidget(Game *_game, QWidget *parent)
    : QGLWidget(parent), GameObject(_game)
{
    cameraAngleX        = 0.0;
    cameraAngleY        = 0.5;
    cameraDistance      = 10;
    cameraMaxDistance   = 20;
    cameraMinDistance   = 2;

    lastMousePos.setX(0);
    lastMousePos.setY(0);

    setMouseTracking(true);
}

GLWidget::~GLWidget()
{
    delete game;
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(700, 500);
}

void GLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective (65.0, width() / height(), 1.0, 1024.0);

    glMatrixMode(GL_MODELVIEW);

    qDebug() << "OpenGL initialized:" << (char*)glGetString(GL_VERSION)
        << (char*)glGetString(GL_VENDOR);
    qDebug() << (char*)glGetString(GL_RENDERER);

    GLfloat light_position[] = { 5.0, 1.0, 0.0, 1.0 };
    //GLfloat white_color[] = { 0.5, 0.5, 0.5, 1.0 };
    //GLfloat black_color[] = { 0.0, 0.0, 0.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_AMBIENT, white_color);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, black_color);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, black_color);
#ifdef GL_LIGHT_MODEL_COLOR_CONTROL
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
#endif

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    game->getBoard()->loadByName("StandardSettlers");
    game->getBoard()->generate();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    updateCameraPos();

    game->render();
}

void GLWidget::updateCameraPos()
{
    glLoadIdentity();
    gluLookAt( 0, 0, cameraDistance,    0, 0, 0, 0, 1, 0);
    gluLookAt( 0, cos(cameraAngleY), sin(cameraAngleY), 0, 0, 0, 0, 1, 0);
    gluLookAt( cos(cameraAngleX), 0, sin(cameraAngleX), 0, 0, 0, 0, 1, 0);
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setPerspective();

    glMatrixMode(GL_MODELVIEW);
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    // reset last mouse position
    if(event->type() & QEvent::MouseButtonPress &&
        event->button() & Qt::LeftButton)
    {
        qDebug() << "Mouse pressed" << event->pos();

        // route event
        if(game->getBoard()->handleMouseClick(event->pos())) return;
    }
    else if(event->type() & QEvent::MouseButtonRelease)
    {
        lastMousePos.setX(0);
        lastMousePos.setY(0);
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(lastMousePos.isNull()) lastMousePos = event->pos();

    // rotate the board
    if(event->buttons() & Qt::RightButton)
    {
        const QPoint _curpos  = event->pos();

        cameraAngleX += ( lastMousePos.x() - _curpos.x() ) * -CAM_X_ROT_DELTA;
        cameraAngleY += ( lastMousePos.y() - _curpos.y() ) *  CAM_Y_ROT_DELTA;

        if(cameraAngleY > CAM_MAX_Y_ANGLE) cameraAngleY = CAM_MAX_Y_ANGLE;
        if(cameraAngleY < CAM_MIN_Y_ANGLE) cameraAngleY = CAM_MIN_Y_ANGLE;

        if(cameraAngleX > 2*M_PI)   cameraAngleX = 0;
        if(cameraAngleX < 0)        cameraAngleX = 2*M_PI;

        updateGL();
    }
    else
    {
        game->getBoard()->handleMouseOver(event->pos());
    }

    lastMousePos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    cameraDistance += (event->delta() < 0) ? CAM_ZOOM_DELTA : -CAM_ZOOM_DELTA;

    if(cameraDistance < cameraMinDistance)
        cameraDistance = cameraMinDistance;

    if(cameraDistance > cameraMaxDistance)
        cameraDistance = cameraMaxDistance;

    updateGL();
}

void GLWidget::setPerspective()
{
    gluPerspective(CAM_FOV, (GLfloat) width() / (GLfloat) height(), 1.0, 1024.0);
}

void GLWidget::beginGLSelection(QPoint pos)
{
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(GL_SELBUF_SIZE, selectionBuffer);
    (void) glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(-1);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(
        (GLdouble) pos.x(),
        (GLdouble) (viewport[3] - pos.y()),
        1.0, 1.0,
        viewport);
    setPerspective();

    glMatrixMode(GL_MODELVIEW);
}

QList<GLuint> GLWidget::endGLSelection()
{
    GLint c;
    GLuint *hit_ptr;
    QList<GLuint> hits;

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glFlush();

    c = glRenderMode(GL_RENDER);

    hit_ptr = (GLuint *)selectionBuffer;
    for(int i = 0; i < c; i++)
    {
        GLuint names = *hit_ptr;
        hit_ptr += 3; // skip z values for now
        for(GLuint j = 0; j < names; ++j)
        {
            hits.append(*hit_ptr);
            hit_ptr++;
        }
    }

    return hits;
}

