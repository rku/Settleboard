
#include "Game.h"
#include "Player.h"
#include "Crossroad.h"
#include "PlayerObject.h"
#include "Roadway.h"

Roadway::Roadway(QVector3D a, QVector3D b, QObject *parent)
    : GLGameModelProxy(parent)
{
    playerObject = NULL;

    setVertices(a,b);

    selectionRectListID = glGenLists(1);
    createSelectionRect();
}

Roadway::~Roadway()
{
    glDeleteLists(selectionRectListID, 1);
}

void Roadway::createSelectionRect()
{
    glNewList(selectionRectListID, GL_COMPILE);

    glTranslatef(0.0f, 0.01f, 0.0f);

    glBegin(GL_LINES);

    glVertex3f(vertexA.x(), vertexA.y(), vertexA.z());
    glVertex3f(vertexB.x(), vertexB.y(), vertexB.z());

    glEnd();

    glEndList();
}

void Roadway::draw()
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->draw();
        return;
    }

    bool selectable = getIsSelectable() &&
        GAME->getBoard()->getIsSelectionModeActive();
    Player *p = GAME->getPlayers().at(0);
    QColor color = (selectable) ? p->getColor() : Qt::black;
    float width = (selectable) ? 5.0f : 2.0f;

    // draw roadway lines
    glPushMatrix();
    GAME->getGLWidget()->qglColor((getIsHighlighted()) ? color.lighter() : color);
    glLineWidth(width);
    glCallList(selectionRectListID);
    glPopMatrix();
}

void Roadway::setVertices(QVector3D a, QVector3D b)
{
    // always start with the vector having the lowest x value
    if(b.x() < a.x()) qSwap(a,b); 

    // add vertices
    vertexA = a;
    vertexB = b;

    // calculate center between a and b, where objects
    // should be placed

    // calculate the vector between a and b
    QVector3D ab = b - a;

    // coords of centerVertex: vector(0->a) + 0.5*vector(a->b)
    centerVertex = a + 0.5 * ab;
}

void Roadway::addTile(HexTile *tile)
{
    if(tiles.contains(tile)) return;

    Q_ASSERT(tiles.size() < 2);
    tiles.append(tile);
    tile->addRoadway(this);
}

void Roadway::addNeighbour(Roadway *roadway)
{
    if(neighbours.contains(roadway)) return;

    Q_ASSERT(neighbours.size() < 4);
    neighbours.append(roadway);
    roadway->addNeighbour(this);
}

void Roadway::addCrossroad(Crossroad *crossroad)
{
    if(crossroads.contains(crossroad)) return;

    Q_ASSERT(crossroads.size() < 2);
    crossroads.append(crossroad);
    crossroad->addRoadway(this);
}

void Roadway::placePlayerObject(PlayerObject *p)
{
    GLGameModelProxy::placePlayerObject(p);

    if(playerObject != NULL)
    {
        playerObject->pointInDirection(vertexB - vertexA);
        playerObject->setPos(centerVertex);
    }
}

// QDataStream operators

QDataStream &operator<<(QDataStream &stream, const RoadwayPtr &obj)
{
    stream << obj.object->getVertexA();
    stream << obj.object->getVertexB();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, RoadwayPtr &obj)
{
    QVector3D v1, v2;

    stream >> v1;
    stream >> v2;

    // find object with vertices v1/v2 
    obj.object = GAME->getBoard()->getRoadwayWithVertices(v1, v2);
    Q_ASSERT(obj.object != NULL);

    return stream;
}

