
#include <QtDebug>

#include "Map.h"

Map::Map(QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing map...";
}

Map::~Map()
{
    while(!tiles.isEmpty()) delete tiles.takeFirst();
}

void Map::createTile(unsigned int x, unsigned int y,
    HexTile::HexTileType type, bool isFixed, int chipNumber,
    HexTile::HexTileType portType)
{
    Q_ASSERT(size.width() > (int)x);
    Q_ASSERT(size.height() > (int)y);

    HexTile *tile = new HexTile(type, x, y, this);
    tile->setHasFixedPosition(isFixed);
    //tile->setHasPort(hasPort);

    if(chipNumber > 1 && chipNumber < 13 && chipNumber != 7)
        tile->setChipNumber(chipNumber);

    tiles.append(tile);
}

