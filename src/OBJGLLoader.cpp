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
    int currentMaterial = -1;
    QList<Material> materials;
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

        // vertex data
        if(!parts.at(0).isEmpty() && parts.at(0).at(0) == 'v')
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

            if(currentMaterial > -1 && currentMaterial < materials.size())
                face.texFilename = materials.at(currentMaterial).texFilename;

            obj.glModelFaces.append(face);
        }
        // material library file
        else if(parts.at(0) == "mtllib")
        {
            materials = loadMaterials(QString("%1/%2")
                .arg(finfo.path()).arg(parts.at(1)));
        }
        // material
        else if(parts.at(0) == "usemtl")
        {
            currentMaterial = -1;
            for(int i = 0; i < materials.size(); ++i)
                if(materials.at(i).name == parts.at(1)) currentMaterial = i;
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

QList<Material> OBJGLLoader::loadMaterials(QString mtlFilename)
{
    QFile file(mtlFilename);
    Material mtl;
    QList<Material> materials;

    if(!file.exists())
    {
        qDebug() << "Material library doesn't exist:" << mtlFilename;
        return materials;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open mtl file:" << mtlFilename << file.error();
        return materials;
    }

    qDebug() << "Loading materials from" << mtlFilename;

    while(!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        QStringList parts = line.split(" ", QString::SkipEmptyParts);

        if(parts.size() < 2) continue;
        if(parts.at(0).startsWith("#")) continue;

        if(parts.at(0) == "newmtl")
        {
            if(!mtl.name.isEmpty()) materials.append(mtl);

            mtl.name = parts.at(1);
        }
        else if(parts.at(0) == "map_Kd")
        {
            mtl.texFilename = parts.last(); 
        }
        else
        {
            qDebug() << "Unhandled material data:" << line;
        }
    }

    if(!mtl.name.isEmpty()) materials.append(mtl);

    file.close(); 

    return materials;
}

