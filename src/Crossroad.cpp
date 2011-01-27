
#include <QtOpenGL>

#include "Game.h"
#include "Roadway.h"
#include "Player.h"
#include "Crossroad.h"

Crossroad::Crossroad(Game *_game, Vertex3f v) : GLGameModel(_game)
{
    selectionCircleListID = glGenLists(1);
    createSelectionCircle();

    setVertex(v);
    setIsHighlighted(false);
}

Crossroad::~Crossroad()
{
    glDeleteLists(selectionCircleListID, 1);
}

void Crossroad::createSelectionCircle()
{
    // create a circle of n triangles
    glNewList(selectionCircleListID, GL_COMPILE);

    int n = 10;
    float r = 0.15f;
    float gamma = 2 * M_PI / n;

    for(int i = 0; i < n; i++)
    {
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(r*qCos((i+1)*gamma), 0.0f, r*qSin((i+1)*gamma));
        glVertex3f(r*qCos( i   *gamma), 0.0f, r*qSin( i   *gamma));
        glEnd();
    }

    glEndList();
}

void Crossroad::drawSelectionCircle()
{
    QGLWidget *widget = game->getGLWidget();
    QColor color = game->getPlayers().at(0)->getColor();

    glPushMatrix();
    widget->qglColor((isHighlighted) ? color.lighter() : color);
    glTranslatef(vertex.x, vertex.y + 0.05f, vertex.z);
    glCallList(selectionCircleListID);
    glPopMatrix();
}

void Crossroad::draw()
{
    if(getIsSelectable()) drawSelectionCircle();

    // draw buildings...
}

void Crossroad::addTile(HexTile *tile)
{
    Q_ASSERT(tiles.size() < 3);

    if(!tiles.contains(tile))
    {
        tiles.append(tile);
        tile->addCrossroad(this);
    }
}

void Crossroad::addNeighbour(Crossroad *neighbour)
{
    Q_ASSERT(neighbours.size() < 3);
    if(!neighbours.contains(neighbour)) neighbours.append(neighbour);
}

void Crossroad::addRoadway(Roadway *roadway)
{
    Q_ASSERT(roadways.size() < 3);

    if(!roadways.contains(roadway))
    {
        roadways.append(roadway);
        roadway->addCrossroad(this);
    }
}

