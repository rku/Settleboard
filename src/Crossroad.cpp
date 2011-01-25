
#include <QtOpenGL>

#include "Game.h"
#include "Roadway.h"
#include "Crossroad.h"

Crossroad::Crossroad(Game *_game, Vertex3f v) : game(_game)
{
    selectionCircleListID = glGenLists(1);
    createSelectionCircle();

    setVertex(v);
}

Crossroad::~Crossroad()
{
    glDeleteLists(selectionCircleListID, 1);
}

void Crossroad::createSelectionCircle()
{
    // create a circle
    glNewList(selectionCircleListID, GL_COMPILE);

    int n = 10;
    float r = 0.15f;
    float gamma = 2 * M_PI / n;

    for(int i = 0; i < n; i++)
    {
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(r*cos((i+1)*gamma), 0.0f, r*sin((i+1)*gamma));
        glVertex3f(r*cos( i   *gamma), 0.0f, r*sin( i   *gamma));
        glEnd();
    }

    glEndList();
}

void Crossroad::drawSelectionCircle()
{
    glPushMatrix();
    game->getGLWidget()->qglColor(Qt::red); // FIXME: use player's color
    glTranslatef(vertex.x, vertex.y + 0.02f, vertex.z);
    glCallList(selectionCircleListID);
    glPopMatrix();
}

void Crossroad::draw()
{
    // draw buildings...
}

void Crossroad::addTile(HexTile *tile)
{
    Q_ASSERT(tiles.size() < 3);
    if(!tiles.contains(tile)) tiles.append(tile);
}

void Crossroad::addNeighbour(Crossroad *neighbour)
{
    Q_ASSERT(neighbours.size() < 3);
    if(!neighbours.contains(neighbour)) neighbours.append(neighbour);
}

void Crossroad::addRoadway(Roadway *roadway)
{
    Q_ASSERT(roadways.size() < 3);
    if(!roadways.contains(roadway)) roadways.append(roadway);
}

