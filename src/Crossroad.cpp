
#include <QtOpenGL>

#include "Game.h"
#include "Roadway.h"
#include "Player.h"
#include "Crossroad.h"
#include "Building.h"

Crossroad::Crossroad(Game *_game, QVector3D v) : GLGameModel(_game)
{
    selectionCircleListID = glGenLists(1);
    createSelectionCircle();

    setVertex(v);
    playerObject = NULL;
    setIsHighlighted(false);
}

Crossroad::~Crossroad()
{
    if(getIsPlayerObjectPlaced()) delete playerObject;
    glDeleteLists(selectionCircleListID, 1);
}

void Crossroad::setVertex(QVector3D v)
{
    vertex = v;
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
    glTranslatef(vertex.x(), vertex.y() + 0.05f, vertex.z());
    glCallList(selectionCircleListID);
    glPopMatrix();
}

void Crossroad::draw()
{
    if(getIsPlayerObjectPlaced())
    {
        playerObject->draw();
        return;
    }

    // if no building is available on this crossroad and
    // it is selectable, draw the selection circle
    if(getIsSelectable()) drawSelectionCircle();
}

void Crossroad::addTile(HexTile *tile)
{
    if(tiles.contains(tile)) return;

    Q_ASSERT(tiles.size() < 3);

    tiles.append(tile);
    tile->addCrossroad(this);
}

void Crossroad::addNeighbour(Crossroad *neighbour)
{
    if(neighbours.contains(neighbour)) return;

    Q_ASSERT(neighbours.size() < 3);
    neighbours.append(neighbour);
    neighbour->addNeighbour(this);
}

void Crossroad::addRoadway(Roadway *roadway)
{
    if(roadways.contains(roadway)) return;

    Q_ASSERT(roadways.size() < 3);

    for(int i = 0; i < roadways.size(); i++)
        roadway->addNeighbour(roadways.at(i));

    roadways.append(roadway);
    roadway->addCrossroad(this);
}

void Crossroad::placePlayerObject(GLGameModel *b)
{
    playerObject = b;

    if(playerObject != NULL)
    {
        playerObject->setPos(vertex);
        playerObject->pointToVertex(QVector3D(0.0, vertex.y(), 0.0));
    }
}

