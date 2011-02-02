
#include "Player.h"
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addStretch();
    this->setLayout(layout);
}

PlayerPanel::~PlayerPanel()
{
}

void PlayerPanel::registerPlayerInfo(QString infoName)
{
    Q_ASSERT(!playerInfos.contains(infoName));
    playerInfos.append(infoName);

    updatePlayerInfos();
}

void PlayerPanel::updatePlayerInfo(Player *player, QString infoName, uint value)
{
    Q_ASSERT(playerInfos.contains(infoName));

    QGroupBox *playerBox = NULL;
    QVBoxLayout *layout = (QVBoxLayout*)this->layout();
    QGridLayout *boxLayout;

    for(int i = 0; i < playerBoxes.size(); i++)
    {
        if(playerBoxes.at(i)->title() == player->getName())
        {
            playerBox = playerBoxes.at(i);
            break;
        }
    }

    if(playerBox == NULL)
    {
        playerBox = new QGroupBox(player->getName(), this);
        playerBox->setLayout(new QGridLayout());
        playerBoxes.append(playerBox);
        updatePlayerInfos();
        layout->insertWidget(0, playerBox);
    }

    boxLayout = (QGridLayout*)playerBox->layout();
    // remember: the last item is the stretch item!
    int row = playerInfos.indexOf(infoName);
    if(row >= 0)
    {
        QLayoutItem *item = boxLayout->itemAtPosition(row, 1);
        Q_ASSERT(item != NULL);
        ((QLabel*)item->widget())->setText(QString("%1").arg(value));
    }
}

void PlayerPanel::updatePlayerInfos()
{
    for(int i = 0; i < playerBoxes.size(); i++)
    {
        QGroupBox *box = playerBoxes.at(i);
        QGridLayout *layout = (QGridLayout*)box->layout();
        int rows = (layout->itemAt(0)!=NULL) ? layout->rowCount() : 0;
        int diff = playerInfos.size() - rows;

        for(; diff > 0; diff--)
        {
            QLabel *descLabel = new QLabel(playerInfos.at(rows) + ":", box);
            QLabel *valueLabel = new QLabel("0", box);
            layout->addWidget(descLabel, rows, 0, Qt::AlignLeft);
            layout->addWidget(valueLabel, rows, 1, Qt::AlignRight);
            rows++;
        }
    }
}

void PlayerPanel::clear()
{
    while(!playerBoxes.isEmpty())
    {
        QGroupBox *box = playerBoxes.takeFirst();

        layout()->removeWidget(box);
        delete box;
    }
}

