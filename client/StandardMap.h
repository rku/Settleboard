
#ifndef STANDARDMAP_H
#define STANDARDMAP_H 1

#include "Map.h"

class StandardMap : public Map
{
    Q_OBJECT

    public:
        StandardMap(QObject *parent = 0);
        ~StandardMap();

        const HexTileList& getTiles() { return tiles; }
};

#endif

