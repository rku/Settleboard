
#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H 1

#include <QtGui>

class ControlPanel : public QDockWidget
{
    Q_OBJECT

    public:
        ControlPanel(const QString &title, QWidget *parent = 0);
        ~ControlPanel();

        void registerAction(const QString name, QAction *action);
        void setActionState(const QString name, bool state);
        void beginCancelMode();
        void endCancelMode();
        void clear();

    protected slots:
        void actionTriggered();
        void cancel();

    protected:
        QToolButton *buttonCancel;
        QMap<QString, QToolButton*> buttons;
};

#endif

