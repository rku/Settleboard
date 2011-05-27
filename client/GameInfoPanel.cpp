
#include "Player.h"
#include "FileManager.h"
#include "GameInfoPanel.h"

GameInfoPanel::GameInfoPanel(const QString &title, QWidget *parent)
    : QDockWidget(parent)
{
    // setup ui
    QGridLayout *l = new QGridLayout();
    QWidget *widget = new QWidget(this);

    diceTextLabel = new QLabel("Rolled:", widget);
    diceTextLabel->setVisible(false);
    die1PixmapLabel = new QLabel(widget);
    die2PixmapLabel = new QLabel(widget);
    currentPlayerLabel = new QLabel("No player", widget);
    currentPlayerLabel->setObjectName("currentPlayerLabel");
    turnLabel = new QLabel("Initial turn", widget);
    turnLabel->setObjectName("turnLabel");

    l->addWidget(turnLabel, 0, 0, 1, 4, Qt::AlignLeft);
    l->addWidget(currentPlayerLabel, 1, 0, 1, 4, Qt::AlignLeft);
    l->addWidget(diceTextLabel, 2, 0, Qt::AlignLeft);
    l->addWidget(die1PixmapLabel, 2, 1, Qt::AlignRight);
    l->addWidget(die2PixmapLabel, 2, 2, Qt::AlignRight);

    l->setColumnStretch(3, 1);
    l->setRowStretch(3, 1);

    widget->setLayout(l);
    setWidget(widget);

    setFixedHeight(110);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));
}

GameInfoPanel::~GameInfoPanel()
{
}

void GameInfoPanel::setDiceValues(uint die1, uint die2)
{
    QString fileNameTemplate("Die%1.png");

    QString filePath = FileManager::getPathOfImage(fileNameTemplate.arg(die1));
    QPixmap die1Pixmap(filePath);
    filePath = FileManager::getPathOfImage(fileNameTemplate.arg(die2));
    QPixmap die2Pixmap(filePath);

    die1PixmapLabel->setToolTip(QString("%1").arg(die1));
    die2PixmapLabel->setToolTip(QString("%1").arg(die2));
    diceTextLabel->setVisible(true);

    if(die1Pixmap.isNull())
    { die1PixmapLabel->setText(die1PixmapLabel->toolTip()); }
    else { die1PixmapLabel->setPixmap(die1Pixmap); }

    if(die2Pixmap.isNull())
    { die2PixmapLabel->setText(die2PixmapLabel->toolTip()); }
    else { die2PixmapLabel->setPixmap(die2Pixmap); }
}

void GameInfoPanel::clearDiceValues()
{
    die1PixmapLabel->clear();
    die2PixmapLabel->clear();
    diceTextLabel->setVisible(false);
}

void GameInfoPanel::setCurrentPlayer(Player *currentPlayer)
{
    QString text = QString("%1's turn").arg(currentPlayer->getName());
    QString style = QString("color: %1").arg(currentPlayer->getColor().lighter(130).name());
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

