/**
 * RSETTLE
 *
 * This file is part of
 * RSETTLE - A board game like settlers.
 * Copyright (C) 2007 Rene Kuettner <rene@bitkanal.net>
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

#include "Game.h"
#include "Player.h"
#include "GameLobby.h" 
#include "NetworkCore.h"

GameLobby::GameLobby(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);
    init();
}

void GameLobby::init()
{
    setModal(true);

    nameLabels.append(ui.labelPlayer1Name);
    nameLabels.append(ui.labelPlayer2Name);
    nameLabels.append(ui.labelPlayer3Name);
    nameLabels.append(ui.labelPlayer4Name);

    addressLabels.append(ui.labelPlayer1Address);
    addressLabels.append(ui.labelPlayer2Address);
    addressLabels.append(ui.labelPlayer3Address);
    addressLabels.append(ui.labelPlayer4Address);

    readyLabels.append(ui.labelPlayer1Ready);
    readyLabels.append(ui.labelPlayer2Ready);
    readyLabels.append(ui.labelPlayer3Ready);
    readyLabels.append(ui.labelPlayer4Ready);

    colorWidgets.append(ui.widgetPlayer1Color);
    colorWidgets.append(ui.widgetPlayer2Color);
    colorWidgets.append(ui.widgetPlayer3Color);
    colorWidgets.append(ui.widgetPlayer4Color);

    connect(ui.lineEditChatInput, SIGNAL(returnPressed()),
        this, SLOT(chatMessageAvailable()));
    connect(ui.checkBoxReady, SIGNAL(stateChanged(int)),
        this, SLOT(readyStateChanged(int)));

    clearPlayerList();
}

void GameLobby::clearPlayerList()
{
    for(int i = 0; i < 4; i++)
    {
        nameLabels[i]->setText("--");
        addressLabels[i]->setText("");
        readyLabels[i]->setText("");
        colorWidgets[i]->setStyleSheet("background: none;");
    }

    numberOfPlayers = 0;
}

void GameLobby::update()
{
    clearPlayerList();
    QList<Player*> players = GAME->getPlayers();
    QList<Player*>::iterator i;
    bool allPlayersReady = true;

    for(i = players.begin(); i != players.end() && numberOfPlayers < 4; ++i)
    {
        Player *p = *i;

        nameLabels[numberOfPlayers]->setText(p->getName());
        addressLabels[numberOfPlayers]->setText("");
        setIsPlayerReady(p->getName(), p->getIsReady());
        if(!p->getIsReady()) allPlayersReady = false;

        QString colorStyle = QString("background: %1;")
            .arg(p->getColor().name());
        colorWidgets[numberOfPlayers]->setStyleSheet(colorStyle);

        numberOfPlayers++;
    }

    // update button states
    bool canStartGame = (numberOfPlayers > 0 && allPlayersReady &&
        GAME->getNetworkCore()->getIsServer());
    ui.buttonStartGame->setEnabled(canStartGame);
}

void GameLobby::setIsPlayerReady(const QString &name, bool b)
{
    uint playerNum = 0;

    // find player
    for(playerNum = 0; playerNum < 4; playerNum++)
        if(nameLabels[playerNum]->text() == name) break;

    Q_ASSERT(playerNum < 4); // fails, if player is not in the list
    readyLabels[playerNum]->setStyleSheet((b) ?"color: green;":"color: red;");
    readyLabels[playerNum]->setText((b) ?"Ready":"Not Ready");
}

void GameLobby::addChatMessage(const QString &message, const QColor &color)
{
    ui.textEditChatOutput->setTextColor(color);
    ui.textEditChatOutput->append(message);
}

void GameLobby::chatMessageAvailable()
{
    GAME->getRules()->pushRuleData("ChatMessage",
        ui.lineEditChatInput->text());
    GAME->getRules()->executeRule("ruleNewChatMessage");
    ui.lineEditChatInput->setText("");
}

void GameLobby::readyStateChanged(int state)
{
    bool isReady = (state == Qt::Checked);

    GAME->getRules()->pushRuleData("ReadyState", isReady);
    GAME->getRules()->executeRule("ruleUpdatePlayerReadyState");
}

