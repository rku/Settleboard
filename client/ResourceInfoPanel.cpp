
#include <QHBoxLayout>

#include "FileManager.h"
#include "ResourceInfoPanel.h"

ResourceInfoPanel::ResourceInfoPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QHBoxLayout *l = new QHBoxLayout();
    QWidget *widget = new QWidget(this);

    l->addStretch();
    widget->setLayout(l);
    setWidget(widget);

    setMinimumHeight(80);
    setMaximumHeight(80);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    setFloating(false);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
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
    iconLabel->setPixmap(QPixmap(iconFile));
    QLabel *amountLabel = new QLabel("x0", this);
    QHBoxLayout *l2 = new QHBoxLayout();
    l2->addWidget(iconLabel,0, Qt::AlignRight);
    l2->addWidget(amountLabel,0, Qt::AlignLeft);
    QWidget *infoWidget = new QWidget(this);
    infoWidget->setLayout(l2);

    resources.insert(name, amountLabel);
    l->addWidget(infoWidget);
    l->addStretch();
}

void ResourceInfoPanel::updateResource(const QString &name, unsigned int amount)
{
    Q_ASSERT(resources.contains(name));

    QLabel *amountLabel = resources.value(name);
    amountLabel->setText(QString("x%1").arg(amount));
}

void ResourceInfoPanel::clear()
{
    QHBoxLayout *l = qobject_cast<QHBoxLayout*>(layout());
    while(l->count() > 1) delete l->takeAt(0); // keep last stretch
}

