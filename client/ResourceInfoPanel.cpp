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

#include "FileManager.h"
#include "ResourceInfoPanel.h"

ResourceInfoPanel::ResourceInfoPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QHBoxLayout *l = new QHBoxLayout();
    QWidget *widget = new QWidget(this);

    l->addStretch();
    l->addStretch();
    l->setSpacing(0);
    widget->setLayout(l);
    setWidget(widget);

    setFloating(false);
    setFixedHeight(80);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setObjectName("resourceInfoPanel");
    setTitleBarWidget(new QWidget(this));
}

ResourceInfoPanel::~ResourceInfoPanel()
{
    clear();
}

void ResourceInfoPanel::registerResource(const QString name)
{
    QHBoxLayout *l = qobject_cast<QHBoxLayout*>(widget()->layout());

    QString iconFile = FileManager::getPathOfImage(
        QString("%1.png").arg(name));
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(iconFile).scaled(QSize(16,16),
        Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel *amountLabel = new QLabel("0x", this);

    QVBoxLayout *l2 = new QVBoxLayout();
    l2->addWidget(iconLabel,0, Qt::AlignCenter);
    l2->addWidget(amountLabel,0, Qt::AlignCenter);

    QWidget *infoWidget = new QWidget(this);
    infoWidget->setObjectName("resourceInfoWidget");
    infoWidget->setToolTip(name);
    infoWidget->setLayout(l2);

    resources.insert(name, amountLabel);
    l->insertWidget(resources.count(), infoWidget);
}

void ResourceInfoPanel::updateResource(const QString &name, unsigned int amount)
{
    Q_ASSERT(resources.contains(name));

    QLabel *amountLabel = resources.value(name);
    amountLabel->setText(QString("%1x").arg(amount));
}

void ResourceInfoPanel::clear()
{
    QHBoxLayout *l = qobject_cast<QHBoxLayout*>(widget()->layout());
    while(l->count() > 2) delete l->takeAt(1)->widget();
}

