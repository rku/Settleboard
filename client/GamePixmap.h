
#ifndef GAMEPIXMAP_H
#define GAMEPIXMAP_H 1

#include <QIcon>
#include <QPixmap>

class GamePixmap : public QPixmap
{
    public:
        GamePixmap(const QString &fileName);
        GamePixmap(const QString &fileName, const QColor&);
        ~GamePixmap();

        void scale(const QSize&);
        void setOverlayColor(const QColor&);

        QIcon toIcon();
};

#endif

