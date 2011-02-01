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

#include "ui_chatwidgetform.h"

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
    delete controlPanel;
    delete chatPanel;
    delete gameInfoPanel;
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
    gameMenu->addSeparator();
    gameMenu->addAction(quitAct);

    settingsMenu = _menuBar->addMenu(tr("&Settings"));
    settingsMenu->addAction(prefsAct);

    helpMenu = _menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    QList<QAction*> actions;
    GameRules *rules;

    rules = game->getRules();
    actions = rules->getActions();

    gameToolBar = addToolBar(tr("Game"));

    for(int i=0; i < actions.size(); ++i)
        gameToolBar->addAction(actions.at(i));
}

void MainWindow::createDockWidgets()
{
    controlPanel = new QDockWidget("Controls", this);
    controlPanel->setAllowedAreas(Qt::NoDockWidgetArea);
    controlPanel->setMinimumHeight(40);
    controlPanel->setMaximumHeight(40);
    controlPanel->setFloating(true);
    controlPanel->setFeatures(QDockWidget::DockWidgetMovable
        | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::NoDockWidgetArea, controlPanel);

    gameInfoPanel = new QDockWidget("Game Info", this);
    gameInfoPanel->setAllowedAreas(Qt::RightDockWidgetArea);
    gameInfoPanel->setMinimumHeight(150);
    gameInfoPanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, gameInfoPanel);

    chatPanel = new QDockWidget("Chat", this);
    chatPanel->setAllowedAreas(Qt::RightDockWidgetArea);
    chatPanel->setMinimumHeight(150);
    chatPanel->setMinimumWidth(300);
    chatPanel->setMaximumWidth(500);
    chatPanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, chatPanel);

    QWidget *widget = new QWidget;
    Ui::ChatWidgetForm ui;
    ui.setupUi(widget);
    chatPanel->setWidget(widget);
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

