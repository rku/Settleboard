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

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

#include "Crossroad.h"
#include "Roadway.h"
#include "Player.h"
#include "TradeOffer.h"
#include "HexTile.h"
#include "FileManager.h"
#include "MainWindow.h"

// Load stylesheet from file
void loadStyleSheet()
{
    QFile data(FileManager::getPathOfStyleSheet("default"));
    QString style;

    if(data.open(QFile::ReadOnly))
    {
        QTextStream styleIn(&data);
        style = styleIn.readAll();
        data.close();
        qApp->setStyleSheet(style);
    }
    else qDebug() << "Cannot load stylesheet" << data.fileName();
}

void registerMetaTypeOperators()
{
    // register stream operators for game types
    qRegisterMetaType<CrossroadPtr>("CrossroadPtr");
    qRegisterMetaTypeStreamOperators<CrossroadPtr>("CrossroadPtr");

    qRegisterMetaType<RoadwayPtr>("RoadwayPtr");
    qRegisterMetaTypeStreamOperators<RoadwayPtr>("RoadwayPtr");

    qRegisterMetaType<PlayerPtr>("PlayerPtr");
    qRegisterMetaTypeStreamOperators<PlayerPtr>("PlayerPtr");

    qRegisterMetaType<HexTilePtr>("HexTilePtr");
    qRegisterMetaTypeStreamOperators<HexTilePtr>("HexTilePtr");

    qRegisterMetaType<TradeOfferPtr>("TradeOfferPtr");
    qRegisterMetaTypeStreamOperators<TradeOfferPtr>("TradeOfferPtr");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Settleboard3D");
    app.setApplicationVersion("0.0.1.beta");
    app.setOrganizationDomain("spaceshore.net");

    app.setWindowIcon(QIcon(":/images/logo-icon.svg"));

    registerMetaTypeOperators();
    loadStyleSheet();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
} 

