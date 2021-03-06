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

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <QtOpenGL>
#include <QList>

struct Texture {
    QString name;
    GLuint id;
    unsigned int width;
    unsigned int height;
    bool loaded;
};

class TextureManager : public QObject
{
    Q_OBJECT

    public:
        TextureManager(QObject *parent = 0);
        ~TextureManager();

        GLuint getTextureId(const QString&);
        const Texture& getTexture(const QString&);

    private:
        QList<Texture> textures;

        const Texture& loadTexture(const QString&);
};

#endif /* TEXTURE_MANAGER_H */

