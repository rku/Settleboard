
#include "Crossroad.h"

Crossroad::Crossroad(Vertex3f v)
{
    northWestCorner = NULL;
    northEastCorner = NULL;
    westCorner      = NULL;
    eastCorner      = NULL;
    southWestCorner = NULL;
    southEastCorner = NULL;

    setVertex(v);
}

Crossroad::~Crossroad()
{
}

void Crossroad::connectToNorthWestCorner(Crossroad *other)
{
    if(northWestCorner == other) return;

    northWestCorner = other;
    if(other != NULL) other->connectToSouthWestCorner(this);
}

void Crossroad::connectToWestCorner(Crossroad *other)
{
    if(westCorner == other) return;

    westCorner = other;
    if(other != NULL) other->connectToEastCorner(this);
}

void Crossroad::connectToNorthEastCorner(Crossroad *other)
{
    if(northEastCorner == other) return;

    northEastCorner = other;
    if(other != NULL) other->connectToNorthWestCorner(this);
}

void Crossroad::connectToSouthWestCorner(Crossroad *other)
{
    if(southWestCorner == other) return;

    southWestCorner = other;
    if(other != NULL) other->connectToNorthEastCorner(this);
}

void Crossroad::connectToEastCorner(Crossroad *other)
{
    if(eastCorner == other) return;

    eastCorner = other;
    if(other != NULL) other->connectToWestCorner(this);
}

void Crossroad::connectToSouthEastCorner(Crossroad *other)
{
    if(southEastCorner == other) return;

    southEastCorner = other;
    if(other != NULL) other->connectToNorthWestCorner(this);
}

