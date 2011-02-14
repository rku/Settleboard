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
    load(FileManager::getPathOfGLObject("NumberChip"), Qt::gray);
    setNumber(number);
    setPosY(0.1);
    setScale(0.25);
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

