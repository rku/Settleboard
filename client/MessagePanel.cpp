
#include "Player.h"
#include "MessagePanel.h"

MessagePanel::MessagePanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QVBoxLayout *l = new QVBoxLayout();
    QWidget *widget = new QWidget(this);

    input = new QLineEdit(widget);
    output = new QTextEdit(widget);
    output->setReadOnly(true);

    l->addWidget(output);
    l->addWidget(input);

    widget->setLayout(l);
    setWidget(widget);
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

    output->setTextColor(player->getColor());
    output->append(format.arg(msg));
}

void MessagePanel::addChatMessage(Player *player, const QString msg)
{
    QString format("# <%1> %2");

    output->setTextColor(player->getColor());
    output->append(format.arg(player->getName()).arg(msg));
}
