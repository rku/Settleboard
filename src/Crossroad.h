
#ifndef BOARDTILECORNER_H
#define BOARDTILECORNER_H 1

#include "GLTypes.h"

class HexTile;

class Crossroad
{
    public:
        Crossroad(Vertex3f);
        ~Crossroad();

        Vertex3f getVertex() { return vertex; }
        void setVertex(Vertex3f v) { vertex = v; }

        const QList<HexTile*> getTiles() { return tiles; }
        void addTile(HexTile *t)
            { Q_ASSERT(tiles.size() < 3); tiles.append(t); }

        void connectToNorthWestCorner(Crossroad*);
        void connectToWestCorner(Crossroad*);
        void connectToNorthEastCorner(Crossroad*);
        void connectToSouthWestCorner(Crossroad*);
        void connectToEastCorner(Crossroad*);
        void connectToSouthEastCorner(Crossroad*);

        Crossroad* getNorthWestCorner() { return northWestCorner; }
        Crossroad* getWestCorner ()     { return westCorner; }
        Crossroad* getNorthEastCorner() { return northEastCorner; }
        Crossroad* getSouthWestCorner() { return southWestCorner; }
        Crossroad* getEastCorner()      { return eastCorner; }
        Crossroad* getSouthEastCorner() { return southEastCorner; }

    private:
        Vertex3f vertex;
        QList<HexTile*> tiles;

        // neighbour corners
        Crossroad *northWestCorner;
        Crossroad *westCorner;
        Crossroad *northEastCorner;
        Crossroad *southWestCorner;
        Crossroad *eastCorner;
        Crossroad *southEastCorner;
};

#endif

