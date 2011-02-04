
#include "Player.h"
#include "MessagePanel.h"

MessagePanel::MessagePanel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout();

    input = new QLineEdit(this);
    output = new QTextEdit(this);
    output->setReadOnly(true);

    l->addWidget(output);
    l->addWidget(input);

    setLayout(l);
}

MessagePanel::~MessagePanel()
{
    delete input;
    delete output;
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

