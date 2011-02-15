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
#include "NetworkCore.h"
#include "GameConnector.h" 

GameConnector::GameConnector(QWidget *parent) : QDialog(parent)
{
    connectTimer.setInterval(5000);
    connectTimer.setSingleShot(false);
    connect(&connectTimer, SIGNAL(timeout()), this, SLOT(updateState()));

    ui.setupUi(this);

    ui.editRemotePort->setValidator(
        new QRegExpValidator(QRegExp("^[1-9][0-9]+$"), this));
    ui.editRemoteHost->setValidator(
        new QRegExpValidator(QRegExp("^[0-9a-zA-Z\\.-]+$"), this));

    connect(this, SIGNAL(finished(int)), this, SLOT(cancel()));
    connect(ui.editRemoteHost, SIGNAL(textChanged(const QString&)),
        this, SLOT(textChanged(const QString&)));
    connect(ui.editRemotePort, SIGNAL(textChanged(const QString&)),
        this, SLOT(textChanged(const QString&)));
    connect(ui.buttonConnect, SIGNAL(clicked()), this, SLOT(connectToHost()));
    connect(ui.buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

void GameConnector::show()
{
    connectTimer.stop();
    attempts = 0;
    ui.groupBox->setEnabled(true);
    ui.buttonCancel->setEnabled(false);
    textChanged("");
    ui.labelStatus->setText("");
    QDialog::show();
}

void GameConnector::textChanged(const QString &s)
{
    Q_UNUSED(s);

    bool b = (!ui.editRemotePort->text().isEmpty() && 
                !ui.editRemoteHost->text().isEmpty());
    ui.buttonConnect->setEnabled(b);
}

void GameConnector::connectToHost()
{
    ui.groupBox->setEnabled(false);
    ui.buttonConnect->setEnabled(false);
    ui.buttonCancel->setEnabled(true);

    attempts++;
    ui.labelStatus->setText(
        QString("Connecting to %1:%2... (%3 of 5)")
        .arg(ui.editRemoteHost->text()).arg(ui.editRemotePort->text())
        .arg(attempts));

    unsigned int port = ui.editRemotePort->text().toUInt();
    GAME->getNetworkCore()->connectToServer(ui.editRemoteHost->text(), port);
    if(!connectTimer.isActive()) connectTimer.start();
}

void GameConnector::updateState()
{
    QAbstractSocket::SocketState state;

    state = GAME->getNetworkCore()->getClientSocket()->state();
    if(state == QAbstractSocket::ConnectedState)
    {
        connectTimer.stop();
        ui.labelStatus->setText("Connected!");
        hide();
        return;
    }

    if(attempts < 5)
    {
        connectToHost();
        return;
    }

    attempts = 0;
    connectTimer.stop();
    GAME->getNetworkCore()->disconnectAll();
    ui.labelStatus->setText("Error: Couldn't connect to server!");
    ui.groupBox->setEnabled(true);
    ui.buttonConnect->setEnabled(true);
    ui.buttonCancel->setEnabled(false);
}

void GameConnector::cancel()
{
    qDebug() << "Connection attempt canceled.";
    connectTimer.stop();
    attempts = 5;
    updateState();
}

