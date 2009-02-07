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

typedef struct _Vertex {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vertex;

OBJGLLoader::OBJGLLoader()
{
}

bool OBJGLLoader::load(QString filename)
{
    QFile file(filename);
    QList<Vertex> vertices;

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

    glBegin(GL_POLYGON);

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
            for(int i = 1; i < parts.size(); i++)
            {
                QStringList faceParts = parts.at(i).split("/");
                if(faceParts.size() > 0)
                {
                    int i = faceParts.at(0).toInt() - 1;

                    glVertex3f( vertices.at(i).x,
                                vertices.at(i).y,
                                vertices.at(i).z);
                } 
            }
        }
        else
        {
            qDebug() << "Unknown data in obj file:" << parts;
        }
    }

    glEnd();
    file.close();

    return true;
}

