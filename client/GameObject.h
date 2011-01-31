
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H 1

class Game;

class GameObject
{
    public:
    GameObject(Game *_game) : game(_game) {}

    protected:
    Game *game;
};

#endif

