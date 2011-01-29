
#ifndef ROADWAY_H
#define ROADWAY_H 1

#include "GLTypes.h"

class HexTile;
class Game;
class Crossroad;

class Roadway : public GLGameModel
{
    public:
        Roadway(Game*, QVector3D a, QVector3D b);
        ~Roadway();

        virtual void draw();

        const QVector3D& getVertexA() { return vertexA; }
        const QVector3D& getVertexB() { return vertexB; }
        void setVertices(QVector3D a, QVector3D b);

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

        QVector3D centerVertex;
        QVector3D vertexA;
        QVector3D vertexB;
        float roadwayAngle;
        GLuint selectionRectListID;
        QList<HexTile*> tiles;
        QList<Roadway*> neighbours;
        QList<Crossroad*> crossroads;
        GLGameModel *playerObject;
};

#endif

