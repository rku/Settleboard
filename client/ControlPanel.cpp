
#include "ControlPanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *l = new QHBoxLayout(this);
    l->addStretch();
    l->addStretch();
    setLayout(l);
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::registerAction(QAction *action)
{
    QToolButton *button = new QToolButton(this);
    QHBoxLayout *l = (QHBoxLayout*)layout();

    Q_ASSERT(l->count() >= 2);

    button->setDefaultAction(action);
    l->insertWidget(l->count() - 1, button);
}

void ControlPanel::clear()
{
    while(!buttons.isEmpty())
    {
        QToolButton *button = buttons.takeFirst();

        layout()->removeWidget(button);
        delete button;
    }
}
