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

class Player;
class GLWidget;
class OBJGLLoader;
class Bank;
class GameRules;
class TextureManager;
class MainWindow;
class NetworkCore;

#define GAME Game::getInstance()

class Game : public QObject
{
    Q_OBJECT

    public:
        ~Game();

        static Game* getInstance();

        enum GameState {NoGameState, PreparingState, PlayingState, FinishedState};
        void setState(GameState st) { state = st; }
        GameState getState() { return state; }

        MainWindow* getMainWindow() { return mainWindow; }
        GLWidget* getGLWidget() { return glWidget; }
        GameRules* getRules() { return rules; }
        TextureManager *getTextureManager() { return textureManager; }
        Board *getBoard() { return board; }
        OBJGLLoader *getOBJGLLoader() { return objGLLoader; }
        QList<Player*>& getPlayers() { return players; }
        Player* getLocalPlayer() { return localPlayer; }
        Bank *getBank() { return bank; }
        NetworkCore *getNetworkCore() { return networkCore; }

    protected:
        Game(QObject *parent = 0);

        GameState state;
        MainWindow *mainWindow;
        GLWidget *glWidget;
        GameRules *rules;
        TextureManager *textureManager;
        Board *board;
        OBJGLLoader *objGLLoader;
        QList<Player*> players;
        Player *localPlayer;
        Bank *bank;
        NetworkCore *networkCore;
};

#endif

