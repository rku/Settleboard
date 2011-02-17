
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

    setAllowedAreas(Qt::TopDockWidgetArea);
    setFloating(false);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setMaximumHeight(90);
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
    iconLabel->setPixmap(QPixmap(iconFile));

    QLabel *amountLabel = new QLabel("0", this);

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
    amountLabel->setText(QString("%1").arg(amount));
}

void ResourceInfoPanel::clear()
{
    QHBoxLayout *l = qobject_cast<QHBoxLayout*>(widget()->layout());
    while(l->count() > 2) delete l->takeAt(1)->widget();
    l->addStretch();
}

