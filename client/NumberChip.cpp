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
    number = 1;
    create();
}

NumberChip::~NumberChip()
{
}

void NumberChip::create()
{
    setIsLightingEnabled(false);
    load(FileManager::getPathOfGLObject("NumberChip"), Qt::gray);
    setTexture("number", "nc4.jpg");
    setPosY(0.1);
    setScale(0.25);
}

