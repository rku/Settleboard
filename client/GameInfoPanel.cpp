
#include "Player.h"
#include "FileManager.h"
#include "GameInfoPanel.h"

GameInfoPanel::GameInfoPanel(const QString &title, QWidget *parent)
    : QDockWidget(parent)
{
    // setup ui
    QGridLayout *l = new QGridLayout();
    QWidget *widget = new QWidget(this);

    diceTextLabel = new QLabel("Dice values:", widget);
    dice1PixmapLabel = new QLabel(widget);
    dice2PixmapLabel = new QLabel(widget);
    currentPlayerLabel = new QLabel("No player", widget);
    currentPlayerLabel->setObjectName("currentPlayerLabel");
    turnLabel = new QLabel("Initial turn", widget);
    turnLabel->setObjectName("turnLabel");

    l->addWidget(diceTextLabel, 0, 0, Qt::AlignLeft);
    l->addWidget(dice1PixmapLabel, 0, 1, Qt::AlignRight);
    l->addWidget(dice2PixmapLabel, 0, 2, Qt::AlignRight);
    l->addWidget(currentPlayerLabel, 1, 0, 1, 3, Qt::AlignLeft);
    l->addWidget(turnLabel, 2, 0, 1, 3, Qt::AlignLeft);

    l->setRowStretch(2, 1);

    widget->setLayout(l);
    setWidget(widget);
}

GameInfoPanel::~GameInfoPanel()
{
}

void GameInfoPanel::setDiceValues(uint dice1, uint dice2)
{
    QString fileNameTemplate("Dice_%1.png");

    QString filePath = FileManager::getPathOfImage(fileNameTemplate.arg(dice1));
    QPixmap dice1Pixmap(filePath);
    filePath = FileManager::getPathOfImage(fileNameTemplate.arg(dice2));
    QPixmap dice2Pixmap(filePath);

    if(dice1Pixmap.isNull())
    { dice1PixmapLabel->setText(QString("%1").arg(dice1)); }
    else { dice1PixmapLabel->setPixmap(dice1Pixmap); }

    if(dice2Pixmap.isNull())
    { dice2PixmapLabel->setText(QString("%1").arg(dice2)); }
    else { dice1PixmapLabel->setPixmap(dice2Pixmap); }
}

void GameInfoPanel::setCurrentPlayer(Player *currentPlayer)
{
    QString text = QString("%1's turn").arg(currentPlayer->getName());
    QString style = QString("color: %1").arg(currentPlayer->getColor().name());
    currentPlayerLabel->setText(text);
    currentPlayerLabel->setStyleSheet(style);
}

void GameInfoPanel::setTurn(unsigned int n)
{
    turnLabel->setText(QString("Turn %1").arg(n));
}

void GameInfoPanel::clear()
{
}

