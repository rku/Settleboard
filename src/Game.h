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

#ifndef GAME_H
#define GAME_H 1

#include "Board.h"

class Player;
class GLWidget;
class OBJGLLoader;
class Bank;
class GameRules;
class TextureManager;
class ResourceManager;
class TradeManager;
class MainWindow;
class NetworkCore;
class GameLobby;

#define GAME Game::getInstance()

class Game : public QObject
{
    Q_OBJECT

    public:
        ~Game();

        static Game* getInstance();

        enum GameState {NoGameState, PreparingState, PlayingState,
            FinishedState, EndingState, DisconnectedState};
        void setState(GameState st) { state = st; }
        GameState getState() { return state; }

        bool reset();

        GameLobby* getLobby() { return gameLobby; }
        MainWindow* getMainWindow() { return mainWindow; }
        GLWidget* getGLWidget() { return glWidget; }
        GameRules* getRules() { return rules; }
        TextureManager *getTextureManager() { return textureManager; }
        ResourceManager *getResourceManager() { return resourceManager; }
        TradeManager *getTradeManager() { return tradeManager; }
        Board *getBoard() { return board; }
        OBJGLLoader *getOBJGLLoader() { return objGLLoader; }
        QList<Player*>& getPlayers() { return players; }
        Player* getLocalPlayer() { return localPlayer; }
        Bank *getBank() { return bank; }
        NetworkCore *getNetworkCore() { return networkCore; }

    protected slots:
        void parseCommandLine();

    protected:
        void init();
        void initLocalPlayer();
        void free();

        Game(QObject *parent = 0);

        GameState state;
        GameLobby *gameLobby;
        MainWindow *mainWindow;
        GLWidget *glWidget;
        GameRules *rules;
        TextureManager *textureManager;
        ResourceManager *resourceManager;
        TradeManager *tradeManager;
        Board *board;
        OBJGLLoader *objGLLoader;
        QList<Player*> players;
        Player *localPlayer;
        Bank *bank;
        NetworkCore *networkCore;
};

#endif

