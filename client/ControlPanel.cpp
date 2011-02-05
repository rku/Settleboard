
#include "ControlPanel.h"

ControlPanel::ControlPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QHBoxLayout *l = new QHBoxLayout();
    QWidget *widget = new QWidget(this);
    l->addStretch();
    l->addStretch();
    widget->setLayout(l);
    setWidget(widget);
}

ControlPanel::~ControlPanel()
{
    clear();
}

void ControlPanel::registerAction(const QString name, QAction *action)
{
    Q_ASSERT(!buttons.contains(name));

    QToolButton *button = new QToolButton(this);
    QHBoxLayout *l = (QHBoxLayout*)widget()->layout();

    Q_ASSERT(l->count() >= 2);

    action->setStatusTip(action->toolTip());
    button->setDefaultAction(action);
    l->insertWidget(l->count() - 1, button);

    buttons.insert(name, button);
}

void ControlPanel::setActionState(const QString name, bool state)
{
    Q_ASSERT(buttons.contains(name));

    QToolButton *button = buttons.value(name);
    button->setEnabled(state);
}

void ControlPanel::clear()
{
    while(!buttons.isEmpty())
    {
        QToolButton *button = buttons.take(buttons.keys().at(0));

        widget()->layout()->removeWidget(button);
        delete button;
    }
}
