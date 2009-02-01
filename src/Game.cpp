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

#include "Game.h"
#include "GameUI.h"
#include "ResourceManager.h"

Game::Game()
{
    textureManager = new TextureManager(this);

    ui = new GameUI(this);

    resourceManager = new ResourceManager(this);

    rules = new GameRules(this);

    board = new Board(this);
    board->loadByName("StandardSettlers");
}

Game::~Game()
{
    delete board;
    delete rules;
    delete ui;
    delete textureManager;
}

void Game::render()
{
    board->render();
    ui->render();
}

