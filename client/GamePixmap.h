
#ifndef GAMEPIXMAP_H
#define GAMEPIXMAP_H 1

#include <QPixmap>

class GamePixmap : public QPixmap
{
    public:
        GamePixmap(const QString &fileName, bool colorate = true);
        ~GamePixmap();

    protected:
        void setOverlayColor(const QColor&);
};

#endif

