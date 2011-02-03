
#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H 1

#include <QtGui>

class ControlPanel : public QWidget
{
    Q_OBJECT

    public:
        ControlPanel(QWidget *parent);
        ~ControlPanel();

        void registerAction(const QString name, QAction *action);
        void setActionState(const QString name, bool state);
        void clear();

    protected:
        QMap<QString, QToolButton*> buttons;
};

#endif

