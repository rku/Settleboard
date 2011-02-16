
#ifndef MAP_H
#define MAP_H 1

#include <QObject>
#include <QList>
#include "HexTile.h"

typedef QList<HexTile*> HexTileList;

class Map : public QObject
{
    Q_OBJECT

    public:
        Map(QObject *parent = 0);
        virtual ~Map();

        virtual const HexTileList& getTiles() { return tiles; }
        virtual void shuffleTiles() {}
        virtual void shuffleNumberChips() {}

        const QSize& getSize() { return size; }
        unsigned int getWidth() { return size.width(); }
        unsigned int getHeight() { return size.height(); }
        const QString& getAuthor() { return author; }
        const QString& getTitle() { return title; }
        const QString& getDescription() { return description; }

    protected:
        void createTile(
            unsigned int x, unsigned int y,
            HexTile::HexTileType, bool fixed = true,
            int chipNumber = -1,
            HexTile::HexTileType portType = HexTile::NoType);
        void setSize(const QSize& s) { size = s; }
        void setAuthor(const QString &s) { author = s; }
        void setTitle(const QString &s) { title = s; }
        void setDescription(const QString &s) { description = s; }

        HexTileList tiles;
        QSize size;
        QString author;
        QString title;
        QString description;
};

#endif

