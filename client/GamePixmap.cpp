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

#include "FileManager.h"
#include "Game.h"
#include "Player.h"
#include "GamePixmap.h"

GamePixmap::GamePixmap(const QString &fileName)
    : QPixmap()
{
    load(FileManager::getPathOfImage(fileName));
}

GamePixmap::GamePixmap(const QString &fileName, const QColor &color)
{
    load(FileManager::getPathOfImage(fileName));
    setOverlayColor(color);
}

GamePixmap::~GamePixmap()
{
}

void GamePixmap::scale(const QSize &newSize)
{
    if(isNull()) return;

    QPixmap p = scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage img = p.toImage();

    detach();
    convertFromImage(img);
}

void GamePixmap::setOverlayColor(const QColor &color)
{
    if(isNull()) return;

    QImage img = toImage();
    Q_ASSERT(!img.isNull());

    int maxVal = qMax(color.red(), qMax(color.green(), color.blue()));
    int red = (color.red() > 0) ? maxVal / color.red() : 0.0;
    int green = (color.green() > 0) ? maxVal / color.green() : 0.0;
    int blue = (color.blue() > 0) ? maxVal / color.blue() : 0.0;

    for(int x = 0; x < img.height(); x++)
    {
        for(int y = 0; y < img.width(); y++)
        {
            QColor col = QColor::fromRgba(img.pixel(x,y));

            col.setRed(col.red() * red);
            col.setGreen(col.green() * green);
            col.setBlue(col.blue() * blue);

            img.setPixel(x, y, col.rgba());
        }
    }

    detach();
    convertFromImage(img);
}

QIcon GamePixmap::toIcon()
{
    return QIcon(*this);
}

