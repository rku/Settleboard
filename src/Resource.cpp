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

#include "Resource.h"
#include "Game.h"

Resource::Resource(Game *_game, const QString _name, const int _initamount)
    : GameObject(_game), name(_name), amount(_initamount)
{
    qDebug() << "Resource" << getName() << "initialized.";
}

unsigned int Resource::add(unsigned int n)
{
    amount += n;

    qDebug() << getName() << "+" << n << "=" << getAmount();

    return getAmount();
}

unsigned int Resource::remove(unsigned int n)
{
    if(n > amount) n = amount;
    amount -= n;

    qDebug() << getName() << "-" << n << "=" << getAmount();

    return getAmount();
}

