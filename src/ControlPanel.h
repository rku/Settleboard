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

#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H 1

#include <QtWidgets>

class ControlPanel : public QDockWidget
{
    Q_OBJECT

    public:
        ControlPanel(const QString &title, QWidget *parent = 0);
        ~ControlPanel();

        void registerAction(const QString name, QAction *action);
        void setActionState(const QString name, bool state);
        void beginCancelMode();
        void endCancelMode();
        void clear();
        void disable();

    protected slots:
        void actionTriggered();
        void cancel();

    protected:
        QToolButton *buttonCancel;
        QMap<QString, QToolButton*> buttons;
};

#endif

