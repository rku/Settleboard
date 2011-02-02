
#ifndef GLGAMEMODELPROXY_H
#define GLGAMEMODELPROXY_H 1

#include "GLGameModel.h"

class PlayerObject;

class GLGameModelProxy : public GLGameModel
{
    public:
        GLGameModelProxy(Game*);
        ~GLGameModelProxy();

        virtual bool getIsPlayerObjectPlaced()
            { return (playerObject != NULL); }
        virtual PlayerObject *getPlayerObject() { return playerObject; }
        virtual void placePlayerObject(PlayerObject*);

        virtual bool getIsSelectable();
        virtual void setIsSelectable(bool);
        virtual void setIsHighlighted(bool);
        virtual void setIsEnabled(bool);

    protected:
        PlayerObject *playerObject;
};

#endif
