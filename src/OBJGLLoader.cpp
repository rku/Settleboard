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

OBJGLLoader::OBJGLLoader(Game *_game)
    : game(_game)
{
}

OBJ OBJGLLoader::getObjectFromCache(QString filename)
{
    QFileInfo info(filename);
    QString name = info.absoluteFilePath();
    OBJ obj;

    for(int i; i < objectCache.size(); ++i)
    {
        if(objectCache.at(i).name == name) return objectCache.at(i);
    }

    return obj;
}

bool OBJGLLoader::load(QString filename)
{
    QFile file(filename);
    QFileInfo finfo(file);
    OBJ obj;

    obj = getObjectFromCache(filename);

    if(!obj.name.isEmpty())
    {
        createGLModel(obj);
        return true;
    }

    if(!file.exists())
    {
        qDebug() << "File not found:" << filename;
        return false;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't read file:" << filename << file.error();
        return false;
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
            Vertex v;

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
                    obj.textureCoords.append(v);
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
            Face face;

            for(int i = 1; i < parts.size(); i++)
            {
                QStringList faceParts = parts.at(i).split("/");
                FaceData fdata;

                if(faceParts.size() > 0)
                    fdata.vertexId = faceParts.at(0).toInt();

                if(faceParts.size() > 1 && !faceParts.at(1).isEmpty())
                    fdata.textureVertexId = faceParts.at(1).toInt();

                if(faceParts.size() > 2 && !faceParts.at(2).isEmpty())
                    fdata.vertexNormalId = faceParts.at(2).toInt();

                face.data.append(fdata);
            }

            obj.faces.append(face);
        }
        // material library file
        else if(parts.at(0) == "mtllib")
        {
            loadMaterials(obj, QString("%1/%2")
                .arg(finfo.path()).arg(parts.at(1)));
        }
        // material
        else if(parts.at(0) == "usemtl")
        {
            Face face;
            face.material = parts.at(1);
            obj.faces.append(face);
        }
        // unhandled data
        else
        {
            qDebug() << "Unhandled data in obj file:" << parts;
        }
    }

    file.close();

    obj.name = finfo.absoluteFilePath();
    objectCache.append(obj);

    createGLModel(obj);

    return true;
}

void OBJGLLoader::loadMaterials(OBJ &obj, QString mtlFilename)
{
    QFile file(mtlFilename);
    Material mtl;

    if(!file.exists())
    {
        qDebug() << "Material library doesn't exist:" << mtlFilename;
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open mtl file:" << mtlFilename << file.error();
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
            if(!mtl.name.isEmpty()) obj.materials.append(mtl);

            mtl.clear();
            mtl.name = parts.at(1);
        }
        else if(parts.at(0) == "Ka" && parts.size() == 4)
        {
            mtl.ka[0] = parts.at(1).toFloat();
            mtl.ka[1] = parts.at(2).toFloat();
            mtl.ka[2] = parts.at(3).toFloat();
        }
        else if(parts.at(0) == "Kd" && parts.size() == 4)
        {
            mtl.kd[0] = parts.at(1).toFloat();
            mtl.kd[1] = parts.at(2).toFloat();
            mtl.kd[2] = parts.at(3).toFloat();
        }
        else if(parts.at(0) == "Ks" && parts.size() == 4)
        {
            mtl.ks[0] = parts.at(1).toFloat();
            mtl.ks[1] = parts.at(2).toFloat();
            mtl.ks[2] = parts.at(3).toFloat();
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

    if(!mtl.name.isEmpty()) obj.materials.append(mtl);

    file.close(); 
}

void OBJGLLoader::setGLMaterial(OBJ &obj, QString name)
{
    Material mtl;

    for(int i = 0; i < obj.materials.size(); ++i)
    {
        if(obj.materials.at(i).name == name)
        {
            mtl = obj.materials.at(i);
            break;
        }
    }

    if(mtl.name.isEmpty())
    {
        qDebug() << "Missing material:" << name;
        return;
    }

    glEnable(GL_TEXTURE_2D);

    if(!mtl.texFilename.isEmpty())
    {
        TextureManager *tm = game->getTextureManager();
        GLuint texId = tm->getTextureId(mtl.texFilename);
        glBindTexture(GL_TEXTURE_2D, texId);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //glMaterialfv(GL_FRONT, GL_AMBIENT,  mtl.ka);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE,  mtl.kd);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.ks);
}

void OBJGLLoader::createGLModel(OBJ &obj)
{
    for(int i = 0; i < obj.faces.size(); ++i)
    {
        Face face = obj.faces.at(i);

        if(!face.material.isEmpty()) setGLMaterial(obj, face.material);

        glBegin(GL_POLYGON);

        for(int f = 0; f < face.data.size(); f++)
        {
            FaceData data = face.data.at(f);
            int vertexId = data.vertexId;
            int normalId = data.vertexNormalId;
            int texVerId = data.textureVertexId;
            Vertex v, n, t;

            if(vertexId == -1 || vertexId > obj.vertices.size()) continue;

            v = obj.vertices.at(data.vertexId - 1);

            if(texVerId > -1 && !texVerId > obj.textureCoords.size())
            {
                t = obj.textureCoords.at(texVerId - 1);
                glTexCoord2f(t.x, t.y);
            }

            if(normalId > -1 && !normalId > obj.vertexNormals.size())
            {
                n = obj.vertexNormals.at(normalId - 1);
                glNormal3f(n.x, n.y, n.z);
            }

            glVertex3f(v.x, v.y, v.z);
        }

        glEnd();
    }
}

