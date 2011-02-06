
#ifndef ROADWAY_H
#define ROADWAY_H 1

#include "GLGameModelProxy.h"

#include "GLTypes.h"

class HexTile;
class Game;
class Crossroad;
class PlayerObject;

class Roadway : public GLGameModelProxy
{
    Q_OBJECT

    public:
        Roadway(QVector3D a, QVector3D b, QObject *parent = 0);
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

        void placePlayerObject(PlayerObject*);

    private:
        void createSelectionRect();

        QVector3D centerVertex;
        QVector3D vertexA;
        QVector3D vertexB;
        GLuint selectionRectListID;
        QList<HexTile*> tiles;
        QList<Roadway*> neighbours;
        QList<Crossroad*> crossroads;
};

Q_DECLARE_METATYPE(Roadway*);

#endif

