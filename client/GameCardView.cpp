
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

    labelName = new QLabel(this);
    labelName->setObjectName("labelName");
    labelName->setAlignment(Qt::AlignCenter);

    labelPixmap = new QLabel(this);
    labelPixmap->setScaledContents(false);
    labelPixmap->setObjectName("labelPixmap");
    labelPixmap->setAlignment(Qt::AlignCenter);

    l->addWidget(labelName);
    l->addWidget(labelPixmap);

    setLayout(l);

    clear();
}

void GameCardView::clear()
{
    labelName->clear();
    labelPixmap->clear();
    setIsSelected(false);
    card = NULL;
    setVisible(false);
}

void GameCardView::setCard(GameCard *c)
{
    card = c;

    labelName->setText(c->name);
    labelPixmap->setToolTip(c->description);
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

    style()->unpolish(labelName);
    style()->polish(labelName);

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

