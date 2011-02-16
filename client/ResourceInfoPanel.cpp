
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
    widget->setLayout(l);
    setWidget(widget);

    setMinimumHeight(75);
    setMaximumHeight(75);
    setAllowedAreas(Qt::TopDockWidgetArea);
    setFloating(false);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setObjectName("resourceInfoPanel");
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
    infoWidget->setObjectName("resourceInfoWidget");
    infoWidget->setToolTip(name);
    infoWidget->setFixedWidth(70);
    infoWidget->setFixedHeight(30);
    infoWidget->setLayout(l2);

    resources.insert(name, amountLabel);
    l->insertWidget(resources.count(), infoWidget);
}

void ResourceInfoPanel::updateResource(const QString &name, unsigned int amount)
{
    Q_ASSERT(resources.contains(name));

    QLabel *amountLabel = resources.value(name);
    amountLabel->setText(QString("x%1").arg(amount));
}

void ResourceInfoPanel::clear()
{
    QHBoxLayout *l = qobject_cast<QHBoxLayout*>(widget()->layout());
    while(l->count() > 2) delete l->takeAt(1)->widget();
    l->addStretch();
}

