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

#ifndef GAME_H
#define GAME_H 1

#include "Board.h"
#include "GLWidget.h"
#include "TextureManager.h"
#include "GameRules.h"

class GameUI;
class Player;
class OBJGLLoader;
class Bank;
class MainWindow;

class Game
{
    public:
        Game();
        ~Game();

        void render();

        void setGLWidget(GLWidget *w) { glWidget = w; }
        GLWidget* getGLWidget() { return glWidget; }

        void setMainWindow(MainWindow *w) { mainWindow = w; }
        MainWindow* getMainWindow() { return mainWindow; }

        void setRules(GameRules *r) { rules = r; }
        GameRules* getRules() { return rules; }

        TextureManager *getTextureManager() { return textureManager; }
        GameUI *getUI() { return ui; }
        Board *getBoard() { return board; }
        OBJGLLoader *getOBJGLLoader() { return objGLLoader; }

        const QList<Player*>& getPlayers() { return players; }

        Bank *getBank() { return bank; }

    protected:
        MainWindow *mainWindow;
        GLWidget *glWidget;
        GameRules *rules;
        TextureManager *textureManager;
        GameUI *ui;
        Board *board;
        OBJGLLoader *objGLLoader;
        QList<Player*> players;
        Bank *bank;
};

#endif

