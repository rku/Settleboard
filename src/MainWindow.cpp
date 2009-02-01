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

MainWindow::MainWindow()
{
    glWidget = new GLWidget;

    setCentralWidget(glWidget);

    uiStyle = new UIStyle();

    setWindowTitle(tr("rSettle"));

    createActions();
    createMenus();
    createToolBars();
    createDockWidgets();
}

void MainWindow::createActions()
{
    quitAct = new QAction(tr("Quit"), this);
    quitAct->setStatusTip(tr("Quit the game."));

    preferencesAct = new QAction(tr("Preferences..."), this);
    preferencesAct->setStatusTip(tr("Open the preferences dialogue..."));

    aboutAct = new QAction(tr("About..."), this);
    aboutAct->setStatusTip(tr("Show about dialogue..."));

    // toolbar
    tradeAct = new QAction(tr("Trade"), this);

    buildSettlementAct = new QAction(tr("Build Settlement"), this);

    buildCityAct = new QAction(tr("Build City"), this);

    buildRoadAct = new QAction(tr("Build Road"), this);

    buildShipAct = new QAction(tr("Build Ship"), this);
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
    gameToolBar = addToolBar(tr("Game"));
    gameToolBar->addAction(tradeAct);
    gameToolBar->addAction(buildSettlementAct);
    gameToolBar->addAction(buildCityAct);
    gameToolBar->addAction(buildRoadAct);
    gameToolBar->addAction(buildShipAct);
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

