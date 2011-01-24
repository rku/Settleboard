
#include <QtOpenGL>

#include "Game.h"
#include "Crossroad.h"

static const double PI = 3.14159265358979323846264338327950288419717;

Crossroad::Crossroad(Game *_game, Vertex3f v) : game(_game)
{
    setVertex(v);
}

Crossroad::~Crossroad()
{
}

void Crossroad::drawCircle()
{
    int n = 20;
    Vertex3f center = vertex, p1, p2;
    float r = 0.2f;
    float gamma = 2 * PI / n;

    p1.y = center.y + 0.05f; p2.y = p1.y;

    game->getGLWidget()->qglColor(Qt::red);

    glPushMatrix();
    glTranslatef(center.x, p1.y, center.z);

    for(int i = 0; i < n; i++)
    {
        p1.x = r * cos(i*gamma);
        p1.z = r * sin(i*gamma);
        p2.x = r * cos((i+1)*gamma);
        p2.z = r * sin((i+1)*gamma);

        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, p1.y, 0.0f);
        glVertex3f(p2.x, p1.y, p2.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glEnd();
    }

    glPopMatrix();
}

void Crossroad::addTile(HexTile *tile)
{
    Q_ASSERT(tiles.size() < 3);
    tiles.append(tile);
}

void Crossroad::addNeighbour(Crossroad *neighbour)
{
    Q_ASSERT(neighbours.size() < 3);
    neighbours.append(neighbour);
}

