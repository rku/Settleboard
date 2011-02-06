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
#include "Player.h"
#include "Bank.h"
#include "NetworkCore.h"
#include "OBJGLLoader.h"

Game::Game(QObject *parent) : QObject(parent)
{
    state = PreparingState;

    textureManager = new TextureManager(this);
    rules = new GameRules(this);
    board = new Board(this);
    objGLLoader = new OBJGLLoader();
    bank = new Bank();
    networkCore = new NetworkCore(this);

    // append test player
    players.append(new Player(NULL, this));
}

Game::~Game()
{
    delete networkCore;
    delete bank;
    delete objGLLoader;
    delete board;
    delete rules;
    delete textureManager;
}

Game* Game::getInstance()
{
    static Game *instance = NULL;

    if(instance == NULL) instance = new Game();
    return instance;
}

