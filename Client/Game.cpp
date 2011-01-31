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
#include "Player.h"
#include "Bank.h"
#include "OBJGLLoader.h"

Game::Game()
{
    localPlayerIndex = 0;
    activePlayerIndex = 0;

    textureManager = new TextureManager(this);
    ui = new GameUI(this);
    rules = new GameRules(this);
    board = new Board(this);
    objGLLoader = new OBJGLLoader();
    players.append(new Player(this));
    bank = new Bank();
}

Game::~Game()
{
    delete bank;
    delete objGLLoader;
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

Player* Game::getNextPlayer()
{
    Q_ASSERT(players.size() > 0);

    int newI = activePlayerIndex + 1;
    if(newI >= players.size()) newI = 0;
    return players.at(newI);
}

Player* Game::getPreviousPlayer()
{
    Q_ASSERT(players.size() > 0);

    int newI = activePlayerIndex + 1;
    if(newI < 0) newI = players.size() - 1;
    return players.at(newI);
}

