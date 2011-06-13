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

#include <QtGui>

#include "GameCardStack.h"

class GameCardSelectionDialog : public QDialog
{
    Q_OBJECT

    public:
        GameCardSelectionDialog(QWidget *parent = 0);
        ~GameCardSelectionDialog();

        const QList<GameCard*>& getSelectedCards() { return selectedCards; }
        void selectCards(GameCardStack *stack, bool secret = false,
            int amount = 0, bool exact = false);

        void setAcceptActionText(QString text);
        void setDescriptionText(QString text);

    protected slots:
        void sceneChanged(const QList<QRectF>&);

    protected:
        void drawScene();
        void updateSelectedCards();

    private:
        int numberOfRequiredCards;
        bool requireExactSelection;
        QList<GameCard*> selectedCards;
        QGraphicsView *graphicsView;
        QGraphicsScene *scene;
        QPushButton *acceptButton;
        QLabel *textLabel;
};

#endif

