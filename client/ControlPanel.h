
#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H 1

#include <QtGui>

class ControlPanel : public QWidget
{
    Q_OBJECT

    public:
        ControlPanel(QWidget *parent);
        ~ControlPanel();

        void registerAction(QAction *action);
        void clear();

    protected:
        QList<QToolButton*> buttons;
};

#endif

