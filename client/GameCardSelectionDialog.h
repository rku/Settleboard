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

#ifndef GAMECARDSELECTIONDIALOG_H
#define GAMECARDSELECTIONDIALOG_H 1

#include <QtWidgets>

#include "GameCardStack.h"

class GraphicsCardSelectionScene;

class GameCardSelectionDialog : public QDialog
{
    Q_OBJECT

    public:
        GameCardSelectionDialog(QWidget *parent = 0);
        ~GameCardSelectionDialog();

        const QList<GameCard*>& getSelectedCards();
        void selectCards(GameCardStack *stack, bool secret = false,
            int amount = 0, bool exact = false, QString type = QString());

        void setAcceptActionText(QString text);
        void setDescriptionText(QString text);

    protected slots:
        void sceneChanged(const QList<QRectF>&);

    private:
        int numberOfRequiredCards;
        bool requireExactSelection;
        QGraphicsView *graphicsView;
        GraphicsCardSelectionScene *scene;
        QPushButton *acceptButton;
        QLabel *textLabel;
};

#endif

