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
#include "GameInfoPanel.h"

MainWindow::MainWindow()
{
    uiStyle = new UIStyle();

    setWindowTitle(tr("Settleboard3D"));

    createDialogs();
    createActions();
    createMenus();
    createDockWidgets();
    createStatusBar();

    initGame();
}

MainWindow::~MainWindow()
{
    delete playerPanel;
    delete messagePanel;
    delete gameInfoPanel;
    delete controlPanel;
}

void MainWindow::initGame()
{
    GLWidget *glWidget;

    game = new Game();
    glWidget = new GLWidget(game);
    setCentralWidget(glWidget);
    game->setGLWidget(glWidget);
    game->setMainWindow(this);
}

void MainWindow::newGame()
{
    createToolBars();
}

void MainWindow::createDialogs()
{
    gameConnector = new GameConnector(game, this);
    gameConnector->setModal(true);

    prefsForm = new PrefsForm(game, this);
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

    playerPanel = new QDockWidget("", this);
    playerPanel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    playerPanel->setFixedWidth(200);
    playerPanel->setFloating(false);
    playerPanel->setFeatures(QDockWidget::DockWidgetMovable);
    playerPanel->setWidget(new PlayerPanel(playerPanel));
    addDockWidget(Qt::LeftDockWidgetArea, playerPanel);

    gameInfoPanel = new QDockWidget("Game Info", this);
    gameInfoPanel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    gameInfoPanel->setMinimumHeight(150);
    gameInfoPanel->setMaximumHeight(150);
    gameInfoPanel->setFeatures(QDockWidget::DockWidgetMovable);
    gameInfoPanel->setWidget(new GameInfoPanel(gameInfoPanel));
    addDockWidget(Qt::RightDockWidgetArea, gameInfoPanel);

    messagePanel = new QDockWidget("Messages", this);
    messagePanel->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    messagePanel->setMinimumHeight(150);
    messagePanel->setMinimumWidth(150);
    messagePanel->setMaximumWidth(400);
    messagePanel->setFeatures(QDockWidget::DockWidgetMovable);
    messagePanel->setWidget(new MessagePanel(messagePanel));
    addDockWidget(Qt::RightDockWidgetArea, messagePanel);

    controlPanel = new QDockWidget("", this);
    controlPanel->setFixedHeight(60);
    controlPanel->setMaximumHeight(60);
    controlPanel->setMinimumHeight(60);
    controlPanel->setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    controlPanel->setAllowedAreas(Qt::TopDockWidgetArea);
    addDockWidget(Qt::TopDockWidgetArea, controlPanel);
    controlPanel->setWidget(new ControlPanel(controlPanel));
}

void MainWindow::createStatusBar()
{
    statusBar();
}

void MainWindow::showConnector()
{
    qDebug() << "showConnector()";
    gameConnector->show();
}

void MainWindow::showPrefs()
{
    qDebug() << "showPrefs()";
    prefsForm->show();
}

void MainWindow::showAbout()
{
    qDebug() << "showAbout()";
}

