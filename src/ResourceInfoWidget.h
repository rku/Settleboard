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

#ifndef RESOURCEINFOWIDGET_H
#define RESOURCEINFOWIDGET_H 1

#include <QtWidgets>

class ResourceInfoWidget : public QWidget
{
    Q_OBJECT

    public:
        enum ResourceInfoDirection {ResourceInfoVerticalDirection,
            ResourceInfoHorizontalDirection};

        ResourceInfoWidget(QWidget *parent = 0,
            ResourceInfoDirection theInfoDirection = ResourceInfoHorizontalDirection,
            ResourceInfoDirection theWidgetDirection = ResourceInfoHorizontalDirection,
            bool editable = false, QSize iconSize = QSize(32,32));
        ~ResourceInfoWidget();

        void setResourceAmount(QString name, int amount);
        int getResourceAmount(QString name);
        const QMap<QString, int> getResourceAmounts();

        void setIsEditable(bool b);

    protected:
        void createUi();
        void resetUi();
        QWidget* createInfoLabel();
        QWidget* createInfoSpinBox();

        ResourceInfoDirection infoDirection;
        ResourceInfoDirection widgetDirection;
        bool isEditable;
        QSize iconSize;
        QMap<QString, QWidget*> resources;
};

#endif

