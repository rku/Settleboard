
#include "Player.h"
#include "FileManager.h"
#include "GameInfoPanel.h"

GameInfoPanel::GameInfoPanel(QWidget *parent) : QWidget(parent)
{
    // setup ui
    QGridLayout *l = new QGridLayout();

    diceTextLabel = new QLabel("Dice values:", this);
    dice1PixmapLabel = new QLabel(this);
    dice2PixmapLabel = new QLabel(this);
    currentPlayerLabel = new QLabel(this);

    l->addWidget(diceTextLabel, 0, 0, Qt::AlignLeft);
    l->addWidget(dice1PixmapLabel, 0, 1, Qt::AlignRight);
    l->addWidget(dice2PixmapLabel, 0, 2, Qt::AlignRight);
    l->addWidget(currentPlayerLabel, 1, 0, 1, 3, Qt::AlignLeft);

    setLayout(l);
}

GameInfoPanel::~GameInfoPanel()
{
    delete diceTextLabel;
    delete dice1PixmapLabel;
    delete dice2PixmapLabel;
    delete currentPlayerLabel;
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

void GameInfoPanel::clear()
{
}

