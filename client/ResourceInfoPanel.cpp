
#include <QHBoxLayout>

#include "FileManager.h"
#include "ResourceInfoPanel.h"

ResourceInfoPanel::ResourceInfoPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QVBoxLayout *l = new QVBoxLayout();
    QWidget *widget = new QWidget(this);

    l->addStretch();
    l->setSpacing(0);
    widget->setLayout(l);
    setWidget(widget);

    setFloating(false);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFixedWidth(80);
    setObjectName("resourceInfoPanel");
    setTitleBarWidget(new QWidget(this));
}

ResourceInfoPanel::~ResourceInfoPanel()
{
    clear();
}

void ResourceInfoPanel::registerResource(const QString name)
{
    QVBoxLayout *l = qobject_cast<QVBoxLayout*>(widget()->layout());

    QString iconFile = FileManager::getPathOfImage(
        QString("%1.png").arg(name));
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(iconFile).scaled(QSize(16,16),
        Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel *amountLabel = new QLabel("0x", this);

    QHBoxLayout *l2 = new QHBoxLayout();
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
    QVBoxLayout *l = qobject_cast<QVBoxLayout*>(widget()->layout());
    while(l->count() > 1) delete l->takeAt(1)->widget();
}

