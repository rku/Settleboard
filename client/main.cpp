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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loadStyleSheet();

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
} 

