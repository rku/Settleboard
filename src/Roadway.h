
#ifndef ROADWAY_H
#define ROADWAY_H 1

#include "GLTypes.h"

class HexTile;
class Game;
class Crossroad;

class Roadway : public GLGameModel
{
    public:
        Roadway(Game*, Vertex3f a, Vertex3f b);
        ~Roadway();

        virtual void draw();

        const QList<Vertex3f>& getVertices() { return vertices; }
        void setVertices(Vertex3f a, Vertex3f b);

        const QList<HexTile*>& getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Roadway*>& getNeighbours() { return neighbours; }
        void addNeighbour(Roadway*);

        const QList<Crossroad*>& getCrossroads() { return crossroads; }
        void addCrossroad(Crossroad*);

        bool getIsPlayerObjectPlaced() { return (playerObject != NULL); }
        void placePlayerObject(GLGameModel*);

    private:
        void createSelectionRect();

        Vertex3f centerVertex;
        GLuint selectionRectListID;
        QList<Vertex3f> vertices;
        QList<HexTile*> tiles;
        QList<Roadway*> neighbours;
        QList<Crossroad*> crossroads;
        GLGameModel *playerObject;
};

#endif

