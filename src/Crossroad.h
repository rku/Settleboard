
#ifndef CROSSROAD_H
#define CROSSROAD_H 1

#include "GLGameModel.h"
#include "GLTypes.h"

class HexTile;
class Game;
class Roadway;

class Crossroad : public GLGameModel
{
    public:
        Crossroad(Game*, Vertex3f);
        ~Crossroad();

        void draw();

        Vertex3f getVertex() { return vertex; }
        void setVertex(Vertex3f v) { vertex = v; }

        const QList<HexTile*> getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Crossroad*> getNeighbours() { return neighbours; }
        void addNeighbour(Crossroad*);

        const QList<Roadway*> getRoadways() { return roadways; }
        void addRoadway(Roadway*);

    private:
        void createSelectionCircle();
        void drawSelectionCircle();

        GLuint selectionCircleListID;
        Vertex3f vertex;
        QList<HexTile*> tiles;
        QList<Crossroad*> neighbours;
        QList<Roadway*> roadways;
};

#endif

