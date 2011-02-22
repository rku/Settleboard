
#include "FileManager.h"
#include "Game.h"
#include "Player.h"
#include "GamePixmap.h"

GamePixmap::GamePixmap(const QString &fileName, bool colorate)
    : QPixmap()
{
    load(FileManager::getPathOfImage(fileName));
    if(colorate) setOverlayColor(GAME->getLocalPlayer()->getColor());
}

GamePixmap::~GamePixmap()
{
}

void GamePixmap::setOverlayColor(const QColor &color)
{
    if(isNull()) return;

    QImage img = toImage();
    Q_ASSERT(!img.isNull());

    int maxVal = qMax(color.red(), qMax(color.green(), color.blue()));
    int red = (color.red() > 0) ? maxVal / color.red() : 0.0;
    int green = (color.green() > 0) ? maxVal / color.green() : 0.0;
    int blue = (color.blue() > 0) ? maxVal / color.blue() : 0.0;

    for(int x = 0; x < img.height(); x++)
    {
        for(int y = 0; y < img.width(); y++)
        {
            QColor col = QColor::fromRgba(img.pixel(x,y));

            col.setRed(col.red() * red);
            col.setGreen(col.green() * green);
            col.setBlue(col.blue() * blue);

            img.setPixel(x, y, col.rgba());
        }
    }

    convertFromImage(img);
}

