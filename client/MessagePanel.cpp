
#include "Player.h"
#include "Game.h"
#include "MessagePanel.h"

MessagePanel::MessagePanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QVBoxLayout *l = new QVBoxLayout();
    QWidget *widget = new QWidget(this);

    input = new QLineEdit(widget);
    connect(input, SIGNAL(returnPressed()),
        this, SLOT(newMessageAvailable()));

    output = new QTextEdit(widget);
    output->setReadOnly(true);

    l->addWidget(output);
    l->addWidget(input);

    widget->setLayout(l);
    setWidget(widget);

    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    setMinimumHeight(250);
    setMinimumWidth(250);
    setMaximumWidth(500);
    setFeatures(QDockWidget::DockWidgetMovable);
    setTitleBarWidget(new QWidget(this));
}

MessagePanel::~MessagePanel()
{
}

void MessagePanel::addSystemMessage(const QString msg)
{
    QString format("! %1");

    output->setTextColor(Qt::white);
    output->append(format.arg(msg));
}

void MessagePanel::addLogMessage(Player *player, const QString msg)
{
    QString format("- %1");

    output->setTextColor(player->getColor().lighter(130));
    output->append(format.arg(msg));
}

void MessagePanel::addChatMessage(Player *player, const QString msg)
{
    QString format("# <%1> %2");

    output->setTextColor(Qt::yellow);
    output->append(format.arg(player->getName()).arg(msg));
}

void MessagePanel::newMessageAvailable()
{
    QString msg = input->text();
    if(!msg.isEmpty())
    {
        input->setText("");
        GAME->getRules()->pushRuleData("ChatMessage", msg);
        GAME->getRules()->executeRule("ruleNewChatMessage");
    }
}

