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

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <QtOpenGL>
#include <QList>

#include "GameObject.h"

struct Texture {
    QString name;
    GLuint id;
    unsigned int width;
    unsigned int height;
};

class TextureManager : public GameObject
{
    public:
        TextureManager(Game*);
        ~TextureManager();

        GLuint getTextureId(const QString&);
        const Texture& getTexture(const QString&);

    private:
        QList<Texture> textures;

        const Texture& loadTexture(const QString&);
};

#endif /* TEXTURE_MANAGER_H */
