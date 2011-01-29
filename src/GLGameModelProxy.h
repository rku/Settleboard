
#ifndef GLGAMEMODELPROXY_H
#define GLGAMEMODELPROXY_H 1

#include "GLGameModel.h"

class GLGameModelProxy : public GLGameModel
{
    public:
        GLGameModelProxy(Game*);
        ~GLGameModelProxy();

        virtual bool getIsPlayerObjectPlaced()
            { return (playerObject != NULL); }
        virtual void placePlayerObject(GLGameModel*);

        virtual bool getIsSelectable();
        virtual void setIsSelectable(bool);
        virtual void setIsHighlighted(bool);
        virtual void setIsEnabled(bool);

    protected:
        GLGameModel *playerObject;
};

#endif

