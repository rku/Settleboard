
#include "Game.h"
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

    connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));

    buttons.insert(name, button);
}

void ControlPanel::setActionState(const QString name, bool state)
{
    Q_ASSERT(buttons.contains(name));

    QToolButton *button = buttons.value(name);
    button->setEnabled(state);
    button->setVisible(state);
}

void ControlPanel::clear()
{
    while(!buttons.isEmpty())
    {
        QToolButton *button = buttons.take(buttons.keys().at(0));

        widget()->layout()->removeWidget(button);
        button->defaultAction()->disconnect(this);
        delete button;
    }
}

void ControlPanel::actionTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(!action) return;

    QString rule = action->data().value<QString>();
    if(rule.isEmpty()) return;

    qDebug() << "Control panel action triggered. Rule:" << rule;
    GAME->getRules()->executeRule(rule);
}

