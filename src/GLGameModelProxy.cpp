
#include "Game.h"
#include "Player.h"
#include "PlayerObject.h"
#include "GLGameModelProxy.h"

GLGameModelProxy::GLGameModelProxy(Game *_game) : GLGameModel(_game)
{
    playerObject = NULL;
}

GLGameModelProxy::~GLGameModelProxy()
{
}

void GLGameModelProxy::placePlayerObject(PlayerObject *po)
{
    playerObject = po;

    if(playerObject != NULL) playerObject->setBaseObject(this);
}

bool GLGameModelProxy::getIsSelectable()
{
    // if a player object is placed, function as a proxy
    if(getIsPlayerObjectPlaced())
    {
        return playerObject->getIsSelectable();
    }

    return GLGameModel::getIsSelectable();
}

void GLGameModelProxy::setIsSelectable(bool b)
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->setIsSelectable(b);
        return;
    }

    GLGameModel::setIsSelectable(b);
}

void GLGameModelProxy::setIsHighlighted(bool b)
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->setIsHighlighted(b);
        return;
    }

    GLGameModel::setIsHighlighted(b);
}

void GLGameModelProxy::setIsEnabled(bool b)
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->setIsEnabled(b);
        return;
    }

    GLGameModel::setIsEnabled(b);
}

