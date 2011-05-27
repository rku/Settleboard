
#include "GameCardPanel.h"

GameCardPanel::GameCardPanel(const QString &title, QWidget *parent)
    : QDockWidget(parent)
{
    // setup ui
    QGridLayout *l = new QGridLayout();
    QWidget *widget = new QWidget(this);

    QLabel *caption = new QLabel("Development Cards:", this);
    l->addWidget(caption, 0, 0, 1, 2);

    cardList = new QListWidget(this);
    l->addWidget(cardList, 1, 0, 1, 2);

    buttonInfo = new QPushButton("Info", this);
    buttonInfo->setEnabled(false);
    l->addWidget(buttonInfo, 2, 0);

    buttonPlay = new QPushButton("Play", this);
    buttonPlay->setEnabled(false);
    l->addWidget(buttonPlay, 2, 1);

    l->setColumnStretch(2, 1);
    l->setRowStretch(3, 1);
    widget->setLayout(l);
    setWidget(widget);

    setTitleBarWidget(new QWidget(this));
}

GameCardPanel::~GameCardPanel()
{
}

