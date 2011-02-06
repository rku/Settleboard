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

#include "TextureManager.h"
#include "FileManager.h"
#include "GLWidget.h"
#include "Game.h"

TextureManager::TextureManager(QObject *parent) : QObject(parent)
{
}

TextureManager::~TextureManager()
{
    // free all the allocated textures in the OpenGL subsystem
    while(!textures.isEmpty())
    {
        Texture tex = textures.takeFirst();
        glDeleteTextures(1, &tex.id);
    }
}

// This function loads a texture and returns
// its opengl texture ID after binding.
const Texture& TextureManager::loadTexture(const QString& filename)
{
    Texture newTex;
    QImage tex;
    QString filepath;

    filepath = FileManager::getPathOfResource("Textures", filename);

    newTex.name   = filename;
    newTex.width  = 0;
    newTex.height = 0;

    glGenTextures(1, &newTex.id);

    glBindTexture(GL_TEXTURE_2D, newTex.id);
    tex = QImage(filepath);
    //newTex.id = Game::getInstance()->getGLWidget()->bindTexture(tex);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // some generic default texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_MIRRORED_REPEAT
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
#endif

    tex = QGLWidget::convertToGLFormat(QImage(filepath));

    if(!tex.isNull())
    {
        newTex.width = tex.width();
        newTex.height = tex.height();

        glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
            GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex.width(), tex.height(),
            GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

        qDebug() << "Loaded texture: " << filepath;
    }
    else
    {   
        qCritical() << "Can't load image file: " << filepath;
    }

    textures.insert(textures.begin(), newTex);

    return getTexture(filename);
}

// This function tries to get the opengl texture
// ID of the texture with the (file)name <name>.
// If it isn't loaded it tries to do so.
GLuint TextureManager::getTextureId(const QString& filename)
{
    const Texture tex = getTexture(filename);
    return tex.id;
}

const Texture& TextureManager::getTexture(const QString& filename)
{
    for(int i = 0; i < textures.size(); ++i)
    {
        if(textures.at(i).name == filename) return textures.at(i);
    }

    return loadTexture(filename);
}

