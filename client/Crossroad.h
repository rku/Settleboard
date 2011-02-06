
#ifndef CROSSROAD_H
#define CROSSROAD_H 1

#include <QObject>

#include "GLGameModelProxy.h"
#include "GLTypes.h"

class HexTile;
class Game;
class Roadway;
class PlayerObject;

class Crossroad : public GLGameModelProxy
{
    Q_OBJECT

    public:
        Crossroad(QVector3D, QObject *parent = 0);
        ~Crossroad();

        virtual void draw();

        QVector3D getVertex() { return vertex; }
        void setVertex(QVector3D);

        const QList<HexTile*> getTiles() { return tiles; }
        void addTile(HexTile*);

        const QList<Crossroad*> getNeighbours() { return neighbours; }
        void addNeighbour(Crossroad*);

        const QList<Roadway*> getRoadways() { return roadways; }
        void addRoadway(Roadway*);

        void placePlayerObject(PlayerObject*);

    private:
        void createSelectionCircle();
        void drawSelectionCircle();

        GLuint selectionCircleListID;
        QVector3D vertex;
        qreal angleToOrigin;
        QList<HexTile*> tiles;
        QList<Crossroad*> neighbours;
        QList<Roadway*> roadways;
};

Q_DECLARE_METATYPE(Crossroad*);

class CrossroadPointer
{
    public:
        CrossroadPointer() {}
        CrossroadPointer(Crossroad *p) { object = p; }
        CrossroadPointer(const CrossroadPointer &c) { object = c.object; }

        friend QDataStream &operator<<(QDataStream&, const CrossroadPointer&);
        friend QDataStream &operator>>(QDataStream&, CrossroadPointer&);

    protected:
        Crossroad *object;
};

Q_DECLARE_METATYPE(CrossroadPointer);

#endif

