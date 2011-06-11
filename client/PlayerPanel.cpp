/**
 * Settleboard3D
 *
 * This file is part of
 * Settleboard3D - A board game like settlers.
 * Copyright (C) 2010, 2011 Rene Kuettner <rene@bitkanal.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Player.h"
#include "FileManager.h"
#include "GamePixmap.h"
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    columns = 1;
    QGridLayout *lt = new QGridLayout();
    QWidget *widget = new QWidget(this);

    widget->setLayout(lt);
    setWidget(widget);

    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    clear();
}

PlayerPanel::~PlayerPanel()
{
}

QGroupBox* PlayerPanel::getPlayerBox(Player *player)
{
    QGroupBox *box;

    if(!playerBoxes.contains(player))
    {
        // create a new one
        box = new QGroupBox(widget());

        QGridLayout *l = (QGridLayout*)widget()->layout();
        QGridLayout *innerL = new QGridLayout();
        l->addWidget(box, 1, playerBoxes.count());
        innerL->setVerticalSpacing(10);
        box->setLayout(innerL);
        playerBoxes.insert(player, box);

        l->setColumnStretch(playerBoxes.count(), 0);
        l->setColumnStretch(playerBoxes.count()+1, 1);
    }
    else box = playerBoxes.value(player);

    QString style = QString("::title { color: %1; }");
    QColor color = (player->getIsDisconnected()) ? 
        Qt::gray : player->getColor().lighter(130);
    box->setStyleSheet(style.arg(color.name()));
    box->setTitle((player->getIsDisconnected())?"Disconnected":player->getName());

    return box;
}

void PlayerPanel::registerPlayerInfo(Player *player, const QString infoName,
    const QString description, const QString iconFile, bool usePlayerColor)
{
    QGroupBox *box = getPlayerBox(player);
    QGridLayout *gl = (QGridLayout*)box->layout();

    // the grid always has one row, even if there's nothing inside
    int row = 0;
    int col = 0;
    while(gl->itemAtPosition(row, col) != 0)
    {
        col++;
        if(col >= (int)(columns * 2))
        {
            row++;
            col = 0;
        }
    }

    // prepare icon
    GamePixmap icon = GamePixmap(iconFile);
    if(usePlayerColor) icon.setOverlayColor(player->getColor());
    icon.scale(QSize(16,16));

    // add new row for the registered info value
    QLabel *iconLabel = new QLabel(box);
    if(!icon.isNull()) iconLabel->setPixmap(icon);
    iconLabel->setToolTip(description);
    QLabel *textLabel = new QLabel(description, box);
    QLabel *valueLabel = new QLabel("0", box);
    valueLabel->setToolTip(description);

    gl->addWidget(iconLabel,  row, col, Qt::AlignRight);
    gl->addWidget(textLabel,  row, col + 1, Qt::AlignLeft);
    gl->addWidget(valueLabel, row, col + 2, Qt::AlignLeft);

    playerInfos.insertMulti(player, infoName);

    gl->setRowStretch(row, 0);
    gl->setRowStretch(row + 1, 1);
}

void PlayerPanel::updatePlayerInfo(Player *player, const QString infoName, int value)
{
    QGroupBox *box = getPlayerBox(player);
    QGridLayout *gl = (QGridLayout*)box->layout();

    // insert objects info
    Q_ASSERT(playerInfos.contains(player));
    Q_ASSERT(playerInfos.values(player).contains(infoName));

    // the list of values is adjacently ordered
    QList<QString> values = playerInfos.values(player);
    int index = values.size() - values.indexOf(infoName) - 1;
    int row = qRound(index / columns);
    int col = qRound(index % columns) * 2 + 2;
    Q_ASSERT(gl->itemAtPosition(row, col) != NULL);
    QLabel *label = (QLabel*)gl->itemAtPosition(row, col)->widget();
    label->setText(QString("%1").arg(value));
}

void PlayerPanel::clear()
{
    playerInfos.clear();

    while(!playerBoxes.isEmpty())
    {
        QGroupBox *box = playerBoxes.take(playerBoxes.keys().at(0));

        widget()->layout()->removeWidget(box);
        delete box;
    }
}

