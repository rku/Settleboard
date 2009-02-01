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

MainWindow::MainWindow()
{
    uiStyle = new UIStyle();

    setWindowTitle(tr("rSettle"));

    createActions();
    createMenus();
    createDockWidgets();

    newGame();
}

void MainWindow::newGame()
{
    GLWidget *glWidget;

    game = new Game();
    glWidget = new GLWidget(game);
    setCentralWidget(glWidget);
    game->setGLWidget(glWidget);

    createToolBars();
}

void MainWindow::createActions()
{
    quitAct = new QAction(tr("Quit"), this);
    quitAct->setStatusTip(tr("Quit the game."));

    preferencesAct = new QAction(tr("Preferences..."), this);
    preferencesAct->setStatusTip(tr("Open the preferences dialogue..."));

    aboutAct = new QAction(tr("About..."), this);
    aboutAct->setStatusTip(tr("Show about dialogue..."));
}

void MainWindow::createMenus()
{
    QMenuBar *_menuBar = menuBar();
    gameMenu = _menuBar->addMenu(tr("&Game"));
    gameMenu->addAction(quitAct);

    settingsMenu = _menuBar->addMenu(tr("&Settings"));
    settingsMenu->addAction(preferencesAct);

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
    controllerWidget = new QDockWidget(this);
    controllerWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    controllerWidget->setStyle(uiStyle);
    controllerWidget->setTitleBarWidget(0);
    controllerWidget->setMinimumSize(QSize(200,200));
    controllerWidget->setMaximumSize(QSize(200,0));
    controllerWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, controllerWidget);
}

