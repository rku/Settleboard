/**
 * Settleboard3D
 *
 * This file is part of
 * Settleboard3D - A board game like settlers.
 * Copyright (C) 2010, 2011 Rene Kuettner <rene@bitkanal.net>
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
#include <QDockWidget>

class GLWidget;
class GameConnector;
class PrefsForm;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

        static MainWindow* getInstance();

        void initGame();
        void newGame();

    private slots:
        void showConnector();
        void startServer();
        void showPrefs();
        void showAbout();

    private:
        void createDialogs();
        void createActions();
        void createMenus();
        void createToolBars();
        void createDockWidgets();
        void createStatusBar();

        static MainWindow *instance;

        GLWidget *glWidget;
        GameConnector *gameConnector;
        PrefsForm *prefsForm;
        QMenu *gameMenu;
        QMenu *settingsMenu;
        QMenu *helpMenu;
        QAction *connectAct;
        QAction *startServerAct;
        QAction *quitAct;
        QAction *prefsAct;
        QAction *aboutAct;
        QToolBar *gameToolBar;
};

#endif

