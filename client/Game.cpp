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
#include "GameCardBrowser.h"

#include "Game.h"

Game::Game(QObject *parent) : QObject(parent)
{
    localPlayer = NULL;
    init();

    // parse the command line asynchronously
    QTimer::singleShot(0, this, SLOT(parseCommandLine()));
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
    gameCardBrowser = new GameCardBrowser(mainWindow);

    initLocalPlayer();
}

void Game::browseLocalGameCards()
{
    gameCardBrowser->close();
    gameCardBrowser->setCardStack(getLocalPlayer()->getCardStack());
    gameCardBrowser->setCardFilter("Development");
    gameCardBrowser->setIsCancelable(true);
    gameCardBrowser->setAcceptButtonText("Play Selected Card");
    gameCardBrowser->show(GameCardBrowser::PlayCardMode);
}

void Game::parseCommandLine()
{
    QStringList args = QCoreApplication::arguments();
    QStringList::iterator i;

    for(i = args.begin(); i != args.end(); ++i)
    {
        QString s = *i;

        if(s.isEmpty()) continue;
        if(s.at(0) != '-') continue;

        // -server
        if(s.endsWith("server", Qt::CaseInsensitive))
        {
            rules->executeRule("ruleStartServer");
            return;
        }

        // -connect <host> [<port>]
        if(s.endsWith("connect", Qt::CaseInsensitive))
        {
            unsigned int port = DEFAULT_NETWORK_PORT;
            if((++i) == args.end()) qFatal("-connect requires argument");
            QString hostname = *i;
            if((++i) != args.end()) port = (*i).toUInt();
            if(port < 1 || port > 65535) qFatal("invalid network port");

            networkCore->connectToServer(hostname, port);
            return;
        }
    }
}

void Game::free()
{
    while(!players.isEmpty()) delete players.takeFirst();
    delete gameCardBrowser;
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

void Game::initLocalPlayer()
{
    Q_ASSERT(localPlayer == NULL);
    localPlayer = new Player(this);
    localPlayer->setIsLocal(true);
    players.append(localPlayer);
    qDebug() << "Local player is" << localPlayer->getName();
}

bool Game::reset()
{
    if(state == PlayingState)
    {
        if(QMessageBox::question(0, "Game Running",
            "Do you really want to quit the current game?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No) == QMessageBox::No) return false;
    }

    qDebug() << "Resetting game state";
    state = EndingState;

    while(!players.isEmpty()) delete players.takeFirst();
    localPlayer = NULL;

    networkCore->disconnectAll();
    bank->reset();
    gameLobby->reset();
    board->reset();
    rules->reset();
    initLocalPlayer();

    state = NoGameState;
    return true;
}

