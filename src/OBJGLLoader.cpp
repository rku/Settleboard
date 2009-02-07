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

OBJGLLoader::OBJGLLoader()
{
}

bool OBJGLLoader::load(QString filename)
{
    QFile file(filename);

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
        if(parts.size() < 4) continue;

        if(parts.at(0) == "v") // vertex data
        {
            Vertex v;

            v.x = parts.at(1).toFloat();
            v.y = parts.at(2).toFloat();
            v.z = parts.at(3).toFloat();

            vertices.append(v);
        }
        else if(parts.at(0) == "f") // faces
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

            faces.append(face);
        }
        else
        {
            qDebug() << "Unknown data in obj file:" << parts;
        }
    }

    file.close();

    createGLModel();

    return true;
}

void OBJGLLoader::createGLModel()
{
    glBegin(GL_POLYGON);

    for(int i = 0; i < faces.size(); ++i)
    {
        Face face = faces.at(i);

        for(int f = 0; f < face.data.size(); f++)
        {
            FaceData data = face.data.at(f);
            Vertex v = vertices.at( data.vertexId - 1 );

            glVertex3f(v.x, v.y, v.z);
        }
    }

    glEnd();
}
