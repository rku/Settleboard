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

#include "OBJReader.h"

OBJReader::OBJReader(QString filename)
{
    if(filename != 0) read(filename);
}

bool OBJReader::read(QString filename)
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
        if(parts.size() < 4 || parts.size() > 5) continue;

        if(parts.at(0) == "v") // vertex data
        {
            vertices.append(parts.at(1).toFloat());
            vertices.append(parts.at(2).toFloat());
            vertices.append(parts.at(3).toFloat());
        }
        else if(parts.at(0) == "f") // faces
        {
            faces.append(parts.size() - 1);
            for(int i = 1; i < parts.size(); i++)
                faces.append(parts.at(i).toUInt());
        }
        else
        {
            qDebug() << "Unknown data in obj file:" << parts;
        }
    }

    file.close();
    return true;
}

QVector<GLfloat> OBJReader::getVertices()
{
    return vertices;
}

QVector<unsigned int> OBJReader::getNormals()
{
    return normals;
}

QVector<unsigned int> OBJReader::getFaces()
{
    return faces;
}

