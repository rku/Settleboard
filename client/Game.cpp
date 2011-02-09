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

#include "Player.h"
#include "Bank.h"
#include "NetworkCore.h"
#include "OBJGLLoader.h"
#include "GLWidget.h"
#include "TextureManager.h"
#include "GameRules.h"
#include "MainWindow.h"
#include "GameLobby.h"

#include "Game.h"

Game::Game(QObject *parent) : QObject(parent)
{
    init();
}

Game::~Game()
{
    free();
}

void Game::init()
{
    state = NoGameState;
    mainWindow = MainWindow::getInstance();

    // initialize game objects
    glWidget = new GLWidget(mainWindow);
    textureManager = new TextureManager(this);
    rules = new GameRules(this);

    board = new Board(this);
    connect(glWidget, SIGNAL(render()),
        board, SLOT(render()), Qt::DirectConnection);
    connect(glWidget, SIGNAL(mousePressed(QMouseEvent*)),
        board, SLOT(handleMouseClick(QMouseEvent*)));
    connect(glWidget, SIGNAL(mouseOver(QMouseEvent*)),
        board, SLOT(handleMouseOver(QMouseEvent*)));

    objGLLoader = new OBJGLLoader();
    bank = new Bank();
    networkCore = new NetworkCore(this);
    gameLobby = new GameLobby(mainWindow);

    // append local player
    localPlayer = new Player(this);
    localPlayer->setIsLocal(true);
    players.append(localPlayer);
    qDebug() << "Local player is" << localPlayer->getName();
}

void Game::free()
{
    while(!players.isEmpty()) delete players.takeFirst();
    delete gameLobby;
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

void Game::end()
{
    if(getState() == EndingState) return; // already called end()

    state = EndingState;
    networkCore->disconnectAll();
    gameLobby->reset();
    bank->reset();
    board->reset();
    rules->reset();
    state = NoGameState;
}

