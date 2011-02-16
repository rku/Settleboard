
#ifndef RESOURCEINFOPANEL_H
#define RESOURCEINFOPANEL_H 1

#include <QtGui>

class ResourceInfoPanel : public QDockWidget
{
    Q_OBJECT

    public:
        ResourceInfoPanel(const QString &title, QWidget *parent = 0);
        ~ResourceInfoPanel();

        void registerResource(const QString name);
        void updateResource(const QString &name, unsigned int amount);
        void clear();

    protected:
        QHash<QString, QLabel*> resources;
};

#endif

