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

#ifndef NUMBER_CHIP_H
#define NUMBER_CHIP_H 1

#include <QtOpenGL>

#include "TextureManager.h"
#include "OpenGLDrawObject.h"


class NumberChip : public OpenGLDrawObject
{
    public:
        NumberChip();
        ~NumberChip();
        void create();

    private:
        int number;
};

#endif /* NUMBER_CHIP_H */

