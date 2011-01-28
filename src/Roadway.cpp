
#include "Game.h"
#include "Player.h"
#include "Crossroad.h"
#include "Roadway.h"

Roadway::Roadway(Game *_game, Vertex3f a, Vertex3f b) : GLGameModel(_game)
{
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

    Vertex3f a = vertices[0], b = vertices[1];
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);

    glEnd();

    glEndList();
}

void Roadway::draw()
{
    Player *p = game->getPlayers().at(0);
    QColor color = (getIsSelectable()) ? p->getColor() : Qt::black;
    float width = (getIsSelectable()) ? 5.0f : 2.0f;

    // draw roadway lines
    glPushMatrix();
    game->getGLWidget()->qglColor((getIsHighlighted()) ? color.lighter() : color);
    glLineWidth(width);
    glCallList(selectionRectListID);
    glPopMatrix();

    // draw roads
}

void Roadway::setVertices(Vertex3f a, Vertex3f b)
{
    // a and b are crossroad positions
    // we shorten the distance by 30% to represent
    // a roadway between a and b

    // calculate the vector between a and b
    /*Vertex3f s;
    s.x = b.x - a.x;
    s.y = b.y - a.y;
    s.z = b.z - a.z;

    s.x *= 0.15; s.y *= 0.15; s.z *= 0.15; // shorten it by 15%

    // new points are a + s and b - s
    a.x += s.x; a.y += s.y; a.z += s.z;
    b.x -= s.x; b.y -= s.y; b.z -= s.z;*/

    // add vertices
    vertices.clear();
    vertices << a << b;
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

