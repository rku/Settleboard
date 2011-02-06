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

#include <QtGui>

#include "MainWindow.h"
#include "GLWidget.h"
#include "UIStyle.h"
#include "Game.h"
#include "GameConnector.h"
#include "PrefsForm.h"
#include "ControlPanel.h"
#include "PlayerPanel.h"
#include "MessagePanel.h"
#include "NetworkCore.h"
#include "GameInfoPanel.h"
#include "GameLobby.h"

MainWindow::MainWindow()
{
    uiStyle = new UIStyle();

    setWindowTitle(tr("Settleboard3D"));

    createDialogs();
    createActions();
    createMenus();
    createStatusBar();
    createDockWidgets();

    initGame();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initGame()
{
    GLWidget *glWidget;

    game = Game::getInstance();
    glWidget = new GLWidget(this);
    game->setGLWidget(glWidget);
    game->setMainWindow(this);
    setCentralWidget(glWidget);
}

void MainWindow::newGame()
{
    createToolBars();
}

void MainWindow::createDialogs()
{
    gameConnector = new GameConnector(this);
    gameConnector->setModal(true);

    prefsForm = new PrefsForm(this);
    prefsForm->setModal(true);
}

void MainWindow::createActions()
{
    connectAct = new QAction(tr("&Connect To Server..."), this);
    connectAct->setShortcut(tr("Ctrl-C"));
    connectAct->setStatusTip(tr("Connect to a game server."));
    connect(connectAct, SIGNAL(triggered()), this, SLOT(showConnector()));

    startServerAct = new QAction(tr("&Start Server..."), this);
    startServerAct->setStatusTip(tr("Start a game server."));
    connect(startServerAct, SIGNAL(triggered()), this, SLOT(startServer()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(tr("Ctrl-Q"));
    quitAct->setStatusTip(tr("Quit the game."));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    prefsAct = new QAction(tr("&Preferences..."), this);
    prefsAct->setStatusTip(tr("Open the preferences dialogue..."));
    connect(prefsAct, SIGNAL(triggered()), this, SLOT(showPrefs()));

    aboutAct = new QAction(tr("&About..."), this);
    aboutAct->setStatusTip(tr("Show about dialogue..."));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(showAbout()));
}

void MainWindow::createMenus()
{
    QMenuBar *_menuBar = menuBar();

    gameMenu = _menuBar->addMenu(tr("&Game"));
    gameMenu->addAction(connectAct);
    gameMenu->addAction(startServerAct);
    gameMenu->addSeparator();
    gameMenu->addAction(quitAct);

    settingsMenu = _menuBar->addMenu(tr("&Settings"));
    settingsMenu->addAction(prefsAct);

    helpMenu = _menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
}

void MainWindow::createDockWidgets()
{
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

void MainWindow::createStatusBar()
{
    statusBar();
}

void MainWindow::showConnector()
{
    //gameConnector->show();
    game->getNetworkCore()->connectToServer("localhost", 1234);
}

void MainWindow::startServer()
{
    //game->getRules()->executeRule("ruleStartGameServer", NULL);
    game->getNetworkCore()->startServer(1234);
    game->getRules()->executeRule("ruleInitGame", game->getPlayers()[0]);
}

void MainWindow::showPrefs()
{
    prefsForm->show();
}

void MainWindow::showAbout()
{
    qDebug() << "showAbout()";
}

