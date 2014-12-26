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

#include <QHBoxLayout>

#include "Game.h"
#include "ResourceManager.h"
#include "ResourceInfoWidget.h"
#include "ResourceInfoPanel.h"

ResourceInfoPanel::ResourceInfoPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QWidget *container = new QWidget(this);
    infoWidget = new ResourceInfoWidget(this,
        ResourceInfoWidget::ResourceInfoVerticalDirection,
        ResourceInfoWidget::ResourceInfoHorizontalDirection,
        false, QSize(24,24));

    QGridLayout *l = new QGridLayout();
    l->setColumnStretch(1, 1);
    l->addWidget(infoWidget, 1, 2);
    l->setColumnStretch(3, 1);
    container->setLayout(l);
    setWidget(container);

    setFloating(false);
    setFixedHeight(90);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setObjectName("resourceInfoPanel");
    setTitleBarWidget(new QWidget(this));
}

ResourceInfoPanel::~ResourceInfoPanel()
{
    clear();
}

void ResourceInfoPanel::update()
{
    ResourceManager *rm = GAME->getResourceManager();
    Player *player = GAME->getLocalPlayer();
    QMap<QString, int> res = rm->getPlayerResources(player);

    foreach(QString name, res.keys())
    {
        infoWidget->setResourceAmount(name, res.value(name));
    }
}

void ResourceInfoPanel::clear()
{
    //infoWidget->clear();
}

