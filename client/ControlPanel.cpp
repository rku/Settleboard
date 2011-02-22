
#include "Game.h"
#include "ControlPanel.h"

ControlPanel::ControlPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QHBoxLayout *l = new QHBoxLayout();
    QWidget *widget = new QWidget(this);

    l->addStretch();

    buttonCancel = new QToolButton(widget);
    buttonCancel->setVisible(false);
    buttonCancel->setText("Cancel");
    l->addWidget(buttonCancel);
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));

    l->addStretch();

    widget->setLayout(l);
    setWidget(widget);

    setFixedHeight(50);
    setMaximumHeight(50);
    setMinimumHeight(50);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setTitleBarWidget(new QWidget(this));
}

ControlPanel::~ControlPanel()
{
    clear();
}

void ControlPanel::beginCancelMode()
{
    // hide all buttons
    QList<QToolButton*> b = buttons.values();
    QList<QToolButton*>::iterator i;

    for(i = b.begin(); i != b.end(); ++i)
        (*i)->setVisible(false);

    // show cancel button
    buttonCancel->setVisible(true);
}

void ControlPanel::endCancelMode()
{
    if(!buttonCancel->isVisible()) return;

    // hide cancel button
    buttonCancel->setVisible(false);

    // show alle buttons that are enabled
    QList<QToolButton*> b = buttons.values();
    QList<QToolButton*>::iterator i;

    for(i = b.begin(); i != b.end(); ++i)
        (*i)->setVisible(true);
}

void ControlPanel::cancel()
{
    endCancelMode();
    GAME->getRules()->executeRule("ruleCancel");
}

void ControlPanel::registerAction(const QString name, QAction *action)
{
    Q_ASSERT(!buttons.contains(name));

    QToolButton *button = new QToolButton(this);
    button->setFixedWidth(48);
    button->setFixedHeight(40);
    button->setIconSize(QSize(32,32));
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

    // disable all buttons; the have to reenabled by rules now
    QStringList b = buttons.keys();
    QStringList::iterator i = b.begin();
    while(i != b.end()) { setActionState(*i, false); i++; }

    QString rule = action->data().value<QString>();
    if(rule.isEmpty()) return;

    repaint();

    qDebug() << "Control panel action triggered. Rule:" << rule;
    GAME->getRules()->executeRule(rule);
}

