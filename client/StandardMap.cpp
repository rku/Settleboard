
#include <QtDebug>

#include "StandardMap.h"

StandardMap::StandardMap(QObject *parent) : Map(parent)
{
    setSize(QSize(7,7));
    setAuthor("Rene Kuettner");
    setTitle("Standard Settlers (2-4 Player)");
    setDescription("The standard settlers map.");

    // row 1
    // 0, 0 -> empty
    // 1, 0 -> empty
    createTile( 2, 0, HexTile::WaterType);
    createTile( 3, 0, HexTile::WaterType);
    createTile( 4, 0, HexTile::WaterType);
    createTile( 5, 0, HexTile::WaterType);
    // 0, 6 -> empty

    // row 2
    // 0, 1 -> empty
    createTile( 1, 1, HexTile::WaterType);
    createTile( 2, 1, HexTile::WheatType, true, 4);
    createTile( 3, 1, HexTile::WoodType, true, 6, HexTile::SheepType);
    createTile( 4, 1, HexTile::WheatType, true, 9);
    createTile( 5, 1, HexTile::WaterType);
    // 6, 1 -> empty

    // row 3
    // 0, 2 -> empty
    createTile( 1, 2, HexTile::WaterType);
    createTile( 2, 2, HexTile::ClayType, true, 2);
    createTile( 3, 2, HexTile::WoodType, true, 5);
    createTile( 4, 2, HexTile::SheepType, true, 12);
    createTile( 5, 2, HexTile::SheepType, true, 4);
    createTile( 6, 2, HexTile::WaterType);

    // row 4
    createTile( 0, 3, HexTile::WaterType);
    createTile( 1, 3, HexTile::SheepType, true, 9);
    createTile( 2, 3, HexTile::ClayType, true, 8);
    createTile( 3, 3, HexTile::DesertType);
    tiles.last()->setHasRobber(true);
    createTile( 4, 3, HexTile::OreType, true, 8);
    createTile( 5, 3, HexTile::SheepType, true, 10);
    createTile( 6, 3, HexTile::WaterType);

    // row 5
    // 0, 4 -> empty
    createTile( 1, 4, HexTile::WaterType);
    createTile( 2, 4, HexTile::WoodType, true, 3); 
    createTile( 3, 4, HexTile::OreType, true, 5); 
    createTile( 4, 4, HexTile::ClayType, true, 10);
    createTile( 5, 4, HexTile::WoodType, true, 11); 
    createTile( 6, 4, HexTile::WaterType);

    // row 6
    // 0, 5 -> empty
    createTile( 1, 5, HexTile::WaterType);
    createTile( 2, 5, HexTile::WheatType, true, 3);
    createTile( 3, 5, HexTile::WheatType, true, 6);
    createTile( 4, 5, HexTile::OreType, true, 11);
    createTile( 5, 5, HexTile::WaterType);
    // 6, 5 -> empty

    // row 7
    // 0, 6 -> empty
    // 1, 6 -> empty
    createTile( 2, 6, HexTile::WaterType);
    createTile( 3, 6, HexTile::WaterType);
    createTile( 4, 6, HexTile::WaterType);
    createTile( 5, 6, HexTile::WaterType);
    // 6,6 -> empty

    qDebug() << "Map initialized:" << getSize()
        << getTitle() << getDescription() << getAuthor();
}

StandardMap::~StandardMap()
{
}

