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

#include "Game.h"
#include "ResourceManager.h"
#include "ResourceInfoWidget.h"

ResourceInfoWidget::ResourceInfoWidget(QWidget *parent,
            ResourceInfoDirection theInfoDirection,
            ResourceInfoDirection theWidgetDirection,
            bool editable,
            QSize theIconSize)
    : QWidget(parent), infoDirection(theInfoDirection),
      widgetDirection(theWidgetDirection), isEditable(editable),
      iconSize(theIconSize)
{
    createUi();
}

ResourceInfoWidget::~ResourceInfoWidget()
{
    resetUi();
}

void ResourceInfoWidget::setResourceAmount(QString name, int amount)
{
    Q_ASSERT(resources.contains(name));

    if(isEditable)
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(resources.value(name));
        spinBox->setValue(amount);
    }

    QLabel *label = static_cast<QLabel*>(resources.value(name));
    label->setText(QString("%1x").arg(amount));
}

int ResourceInfoWidget::getResourceAmount(QString name)
{
    Q_ASSERT(resources.contains(name));

    if(isEditable)
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(resources.value(name));
        return spinBox->value();
    }

    QLabel *label = static_cast<QLabel*>(resources.value(name));
    return label->text().toInt();
}

void ResourceInfoWidget::setIsEditable(bool b)
{
    isEditable = b;
    createUi();
}

void ResourceInfoWidget::createUi()
{
    ResourceManager *rm = GAME->getResourceManager();
    const QStringList res = rm->getResources();

    resetUi();

    QBoxLayout *widgetLayout = 
        (widgetDirection == ResourceInfoVerticalDirection) ?
        static_cast<QBoxLayout*>(new QVBoxLayout()) :
        static_cast<QBoxLayout*>(new QHBoxLayout());

    foreach(QString name, res)
    {
        QBoxLayout *l =
            (infoDirection == ResourceInfoVerticalDirection) ?
            static_cast<QBoxLayout*>(new QVBoxLayout()) : 
            static_cast<QBoxLayout*>(new QHBoxLayout());

        // icon
        QString iconFile = rm->getResourceIconFilename(name);
        QLabel *icon = new QLabel(this);
        icon->setPixmap(QPixmap(iconFile).scaled(iconSize,
            Qt::KeepAspectRatio, Qt::SmoothTransformation));
        icon->setAlignment((infoDirection == ResourceInfoVerticalDirection) ?
            Qt::AlignCenter | Qt::AlignVCenter :
            Qt::AlignRight | Qt::AlignVCenter);
        icon->setToolTip(name);
        l->addWidget(icon);

        // info
        QWidget *info = (isEditable) ? createInfoSpinBox() : createInfoLabel();
        l->addWidget(info);

        Q_ASSERT(!resources.contains(name));
        resources.insert(name, info);

        widgetLayout->addLayout(l);
    }

    setLayout(widgetLayout);
}

QWidget* ResourceInfoWidget::createInfoLabel()
{
    QLabel *label = new QLabel("0x", this);
    label->setAlignment((infoDirection == ResourceInfoVerticalDirection) ?
        Qt::AlignCenter | Qt::AlignVCenter : Qt::AlignLeft | Qt::AlignVCenter);

    return static_cast<QWidget*>(label);
}

QWidget* ResourceInfoWidget::createInfoSpinBox()
{
    QSpinBox *spin = new QSpinBox(this);

    return static_cast<QSpinBox*>(spin);
}

void ResourceInfoWidget::resetUi()
{
    while(!resources.isEmpty())
    {
        delete resources.take(resources.keys().first());
    }

    while(children().count() > 0) delete children().first();
}

