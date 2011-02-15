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
    ui.setupUi(this);

    ui.editRemotePort->setValidator(
        new QRegExpValidator(QRegExp("^[1-9][0-9]+$"), this));
    ui.editRemoteHost->setValidator(
        new QRegExpValidator(QRegExp("^[0-9a-zA-Z\\.-]+$"), this));

    connect(ui.editRemoteHost, SIGNAL(textChanged(const QString&)),
        this, SLOT(textChanged(const QString&)));
    connect(ui.editRemotePort, SIGNAL(textChanged(const QString&)),
        this, SLOT(textChanged(const QString&)));
    connect(ui.buttonConnect, SIGNAL(clicked()), this, SLOT(connectToHost()));
    connect(ui.buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

void GameConnector::show()
{
    ui.groupBox->setEnabled(true);
    ui.buttonCancel->setEnabled(true);
    textChanged("");
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

    hide();

    unsigned int port = ui.editRemotePort->text().toUInt();
    GAME->getNetworkCore()->connectToServer(ui.editRemoteHost->text(), port);
}

void GameConnector::cancel()
{
    hide();
}

