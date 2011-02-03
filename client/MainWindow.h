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
#include <QDockWidget>

class GLWidget;
class GameConnector;
class PrefsForm;
class UIStyle;
class Game;
class ControlPanel;
class PlayerPanel;
class ChatPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

        void initGame();
        void newGame();

        ControlPanel *getControlPanel()
            { return (ControlPanel*)controlPanel->widget(); }
        PlayerPanel *getPlayerPanel()
            { return (PlayerPanel*)playerPanel->widget(); }
        ChatPanel* getChatPanel()
            { return (ChatPanel*)chatPanel->widget(); }

    private slots:
        void showConnector();
        void showPrefs();
        void showAbout();

    private:
        void createDialogs();
        void createActions();
        void createMenus();
        void createToolBars();
        void createDockWidgets();
        void createStatusBar();

        QDockWidget *playerPanel;
        QDockWidget *chatPanel;
        QDockWidget *gameInfoPanel;
        QDockWidget *controlPanel;
        Game *game;
        GLWidget *glWidget;
        GameConnector *gameConnector;
        PrefsForm *prefsForm;
        UIStyle *uiStyle;
        QMenu *gameMenu;
        QMenu *settingsMenu;
        QMenu *helpMenu;
        QAction *connectAct;
        QAction *quitAct;
        QAction *prefsAct;
        QAction *aboutAct;
        QToolBar *gameToolBar;
};

#endif

