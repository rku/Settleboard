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

#include <stdlib.h>

#include "FileManager.h"
#include "Game.h"
#include "NumberChip.h"

NumberChip::NumberChip(QObject *parent) : GLGameModel(parent)
{
    number = 0;
    create();
}

NumberChip::~NumberChip()
{
}

void NumberChip::create()
{
    setIsLightingEnabled(false);
    load(FileManager::getPathOfGLObject("numberchip"), Qt::gray);
    setNumber(number);
    setScale(0.25);

    // set a random rotation angle
    float angle = ( (qrand() * 359.0) / RAND_MAX ) + 1;
    setAngleY(angle);
}

void NumberChip::setNumber(unsigned int n)
{
    if(n > 1 && n < 13 && n != 7)
    {
        number = n;
        setTexture("number", QString("nc%1.jpg").arg(number));
    }
}

void NumberChip::draw()
{
    if(number < 2 || number > 12 || number == 7) return;
    GLGameModel::draw();
}

