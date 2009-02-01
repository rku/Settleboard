/**
 * RSETTLE
 *
 * This file is part of
 * rSETTLE - A board game like settlers.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

class GLWidget;
class UIStyle;
class Game;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        void newGame();

    private:
        void createActions();
        void createMenus();
        void createToolBars();
        void createDockWidgets();

        Game *game;
        GLWidget *glWidget;
        UIStyle *uiStyle;
        QMenu *gameMenu;
        QMenu *settingsMenu;
        QMenu *helpMenu;
        QAction *quitAct;
        QAction *preferencesAct;
        QAction *aboutAct;
        QToolBar *gameToolBar;
        QDockWidget *controllerWidget;
};

#endif

