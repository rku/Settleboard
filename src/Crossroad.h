
#ifndef CROSSROAD_H
#define CROSSROAD_H 1

#include "GLGameModel.h"
#include "GLTypes.h"

class HexTile;
class Game;
class Roadway;
class Building;

class Crossroad : public GLGameModel
{
    public:
        Crossroad(Game*, QVector3D);
        ~Crossroad();

        void draw();

        QVector3D getVertex() { return vertex; }
        void setVertex(QVector3D v) { vertex = v; }

        const QList<HexTile*> getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Crossroad*> getNeighbours() { return neighbours; }
        void addNeighbour(Crossroad*);

        const QList<Roadway*> getRoadways() { return roadways; }
        void addRoadway(Roadway*);

        bool getIsPlayerObjectPlaced() { return (playerObject != NULL); }
        void placePlayerObject(GLGameModel*);

    private:
        void createSelectionCircle();
        void drawSelectionCircle();

        GLuint selectionCircleListID;
        QVector3D vertex;
        GLGameModel *playerObject;
        QList<HexTile*> tiles;
        QList<Crossroad*> neighbours;
        QList<Roadway*> roadways;
};

#endif

