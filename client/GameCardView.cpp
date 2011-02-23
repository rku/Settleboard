
#include <QStyle>
#include <QVBoxLayout>
#include <QtDebug>

#include "FileManager.h"
#include "GameCardView.h"

GameCardView::GameCardView(QWidget *parent) : QWidget(parent)
{
    isSelected = false;
    card = NULL;

    QVBoxLayout *l = new QVBoxLayout();

    labelPixmap = new QLabel(this);
    labelPixmap->setScaledContents(false);
    labelPixmap->setObjectName("labelPixmap");
    labelPixmap->setAlignment(Qt::AlignCenter);

    l->addWidget(labelPixmap);

    setLayout(l);

    clear();
}

void GameCardView::clear()
{
    labelPixmap->clear();
    setIsSelected(false);
    card = NULL;
    setVisible(false);
}

void GameCardView::setCard(GameCard *c)
{
    card = c;

    labelPixmap->setToolTip(c->name);
    labelPixmap->setPixmap(c->pixmap);
    setVisible(true);
}

void GameCardView::setIsSelected(bool b)
{
    isSelected = false;
    if(!card) return;

    isSelected = b;
    if(b) { emit selected(); }

    style()->unpolish(this);
    style()->polish(this);

    style()->unpolish(labelPixmap);
    style()->polish(labelPixmap);
}

void GameCardView::mousePressEvent(QMouseEvent *event)
{
    if(event->type() & QEvent::MouseButtonPress &&
        event->button() & Qt::LeftButton)
    {
        setIsSelected(true);
    }
}

