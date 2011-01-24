
#ifndef BOARDTILECORNER_H
#define BOARDTILECORNER_H 1

#include "GLTypes.h"

class HexTile;
class Game;

class Crossroad
{
    public:
        Crossroad(Game*, Vertex3f);
        ~Crossroad();

        void drawCircle();

        Vertex3f getVertex() { return vertex; }
        void setVertex(Vertex3f v) { vertex = v; }

        const QList<HexTile*> getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Crossroad*> getNeighbours() { return neighbours; }
        void addNeighbour(Crossroad*);

    private:
        Game *game;
        Vertex3f vertex;
        QList<HexTile*> tiles;
        QList<Crossroad*> neighbours;
};

#endif

