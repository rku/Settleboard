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

#include "OBJGLLoader.h"
#include "TextureManager.h"
#include "Game.h"

OBJGLLoader::OBJGLLoader()
{
}

OBJGLLoader::~OBJGLLoader()
{
}

int OBJGLLoader::getOBJCacheId(QString filename)
{
    QFileInfo info(filename);
    QString name = info.absoluteFilePath();

    for(int i = 0; i < objCache.size(); ++i)
    {
        OBJ obj = objCache.at(i);
        if(obj.name == name) return i;
    }

    return -1;
}

OBJ& OBJGLLoader::getOBJByCacheId(int id)
{
    Q_ASSERT(id > -1 && id < objCache.size());
    return objCache[id];
}

OBJ *OBJGLLoader::load(QString filename)
{
    QFile file(filename);
    QFileInfo finfo(file);
    QString texFilename;
    OBJ obj;
    int id;

    // try to find the model in the cache
    if((id = getOBJCacheId(filename)) > -1)
        return &getOBJByCacheId(id);

    qDebug() << "Loading object model:" << filename;

    if(!file.exists())
    {
        qDebug() << "File not found:" << filename;
        return NULL;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't read file:" << filename << file.error();
        return NULL;
    }

    while(!file.atEnd())
    {
        QString line;
        QStringList parts;

        line = file.readLine().trimmed();
        if(line.startsWith("#")) continue;

        parts = line.split(" ", QString::SkipEmptyParts);
        if(parts.size() < 2) continue;

        // vertex group
        if(parts.at(0) == "vgroup")
        {
            if(parts.size() >= 3)
            {
                GLVertexGroup vertexGroup;

                vertexGroup.name = parts.at(1);
                qDebug() << vertexGroup.name;

                for(int i = 2; i < parts.size(); i++)
                    vertexGroup.indices.append(parts.at(i).toInt());

                obj.vertexGroups.append(vertexGroup);
            }

        }
        // vertex data
        else if(!parts.at(0).isEmpty() && parts.at(0).at(0) == 'v')
        {
            Vertex3f v;

            if(parts.size() > 1)
                v.x = parts.at(1).toFloat();
            if(parts.size() > 2)
                v.y = parts.at(2).toFloat();
            if(parts.size() > 3)
                v.z = parts.at(3).toFloat();

            if(parts.at(0).size() == 1)
            {
                obj.vertices.append(v);
            }
            else switch(parts.at(0).at(1).toAscii())
            {
                case 't':
                    Vertex2f v2; v2.x = v.x; v2.y = v.y;
                    obj.textureCoords.append(v2);
                    break;
                case 'n':
                    obj.vertexNormals.append(v);
                    break;
                default:
                    qDebug() << "Unknown vertex data:" << line;
                    break;
            }
        }
        // faces
        else if(parts.at(0) == "f")
        {
            GLModelFace face;

            for(int i = 1; i < parts.size(); i++)
            {
                QStringList faceParts = parts.at(i).split("/");

                if(faceParts.size() > 0)
                    face.vertexIds.append(faceParts.at(0).toInt() - 1);

                if(faceParts.size() > 1 && !faceParts.at(1).isEmpty())
                    face.textureCoordIds.append(faceParts.at(1).toInt() - 1);

                if(faceParts.size() > 2 && !faceParts.at(2).isEmpty())
                    face.vertexNormalIds.append(faceParts.at(2).toInt() - 1);
            }

            if(!texFilename.isEmpty())
                face.texFilename = texFilename;

            obj.glModelFaces.append(face);
        }
        // material
        else if(parts.at(0) == "usemtl" && parts.size() == 2)
        {
            QStringList matNameParts = parts.at(1).split("_");
            if(matNameParts.isEmpty() || matNameParts.last().isEmpty())
            { texFilename = QString(); }
            else
            { texFilename = matNameParts.last(); }
        }
        // unhandled data
        else
        {
            qDebug() << "Unhandled data in obj file:" << parts;
        }
    }

    file.close();

    // add model to cache
    obj.name = finfo.absoluteFilePath();
    id = objCache.size();
    objCache.append(obj);

    return &getOBJByCacheId(id);
}

