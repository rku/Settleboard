
#include "ChatPanel.h"

ChatPanel::ChatPanel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout();

    input = new QLineEdit(this);
    output = new QTextEdit(this);
    output->setReadOnly(true);

    l->addWidget(output);
    l->addWidget(input);

    setLayout(l);
}

ChatPanel::~ChatPanel()
{
    delete input;
    delete output;
}
