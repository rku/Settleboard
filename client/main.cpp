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

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

#include "Crossroad.h"
#include "Roadway.h"
#include "GLGameModel.h"
#include "Player.h"
#include "FileManager.h"
#include "MainWindow.h"

// Load stylesheet from file
void loadStyleSheet()
{
    QFile data(FileManager::getPathOfStyleSheet("Default"));
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

    qRegisterMetaType<GLGameModelPtr>("GLGameModelPtr");
    qRegisterMetaTypeStreamOperators<GLGameModelPtr>("GLGameModelPtr");

    qRegisterMetaType<PlayerPtr>("PlayerPtr");
    qRegisterMetaTypeStreamOperators<PlayerPtr>("PlayerPtr");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Settleboard3D");
    app.setApplicationVersion("0.0.1.beta");
    app.setOrganizationDomain("bitkanal.net");

    registerMetaTypeOperators();
    loadStyleSheet();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
} 

