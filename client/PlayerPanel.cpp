
#include "Player.h"
#include "FileManager.h"
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    QWidget *widget = new QWidget(this);

    layout->addStretch();
    widget->setLayout(layout);
    setWidget(widget);
}

PlayerPanel::~PlayerPanel()
{
}

QGroupBox* PlayerPanel::getPlayerBox(Player *player)
{
    QGroupBox *box;

    if(!playerBoxes.contains(player))
    {
        // create a new one
        box = new QGroupBox(widget());

        QVBoxLayout *l = (QVBoxLayout*)widget()->layout();
        Q_ASSERT(l->count() > 0); // we expect at least a stretcher
        l->insertWidget(l->count() - 1, box);
        box->setLayout(new QGridLayout());
        playerBoxes.insert(player, box);
    }
    else box = playerBoxes.value(player);

    QString style = QString("::title { color: %1; }");
    QColor color = (player->getIsDisconnected()) ? Qt::gray : player->getColor();
    box->setStyleSheet(style.arg(color.name()));
    box->setTitle((player->getIsDisconnected())?"Disconnected":player->getName());

    return box;
}

void PlayerPanel::registerPlayerInfo(Player *player, const QString infoName,
    const QString description, const QString iconFile, bool usePlayerColor)
{
    QGroupBox *box = getPlayerBox(player);
    QGridLayout *gl = (QGridLayout*)box->layout();

    // the grid always has one row, even if there's nothing inside
    int rows = (gl->itemAt(0) != NULL) ? gl->rowCount() : 0;

    // prepare icon
    QString iconPath = FileManager::getPathOfImage(iconFile);
    QImage icon = QImage(iconPath).convertToFormat(QImage::Format_Indexed8);
    QPixmap pixmap;
    if(!icon.isNull())
    {
        if(usePlayerColor) icon.setColor(0, player->getColor().rgb());
        pixmap.convertFromImage(icon.scaledToHeight(12));
    }

    // add new row for the registered info value
    QLabel *textLabel = new QLabel(description, box);
    QLabel *iconLabel = new QLabel(box);
    if(!pixmap.isNull()) iconLabel->setPixmap(pixmap);
    QLabel *valueLabel = new QLabel("0", box);

    gl->addWidget(iconLabel,  rows, 0, Qt::AlignLeft);
    gl->addWidget(textLabel,  rows, 1, Qt::AlignLeft);
    gl->addWidget(valueLabel, rows, 2, Qt::AlignRight);

    playerInfos.insertMulti(player, infoName);
}

void PlayerPanel::updatePlayerInfo(Player *player, const QString infoName, int value)
{
    QGroupBox *box = getPlayerBox(player);
    QGridLayout *gl = (QGridLayout*)box->layout();

    // insert objects info
    Q_ASSERT(playerInfos.contains(player));
    Q_ASSERT(playerInfos.values(player).contains(infoName));

    QList<QString> values = playerInfos.values(player);
    int row = values.size() - values.indexOf(infoName) - 1;
    Q_ASSERT(gl->itemAtPosition(row, 2) != NULL);
    QLabel *label = (QLabel*)gl->itemAtPosition(row, 2)->widget();
    label->setText(QString("%1").arg(value));
}

void PlayerPanel::clear()
{
    playerInfos.clear();

    while(!playerBoxes.isEmpty())
    {
        QGroupBox *box = playerBoxes.take(playerBoxes.keys().at(0));

        widget()->layout()->removeWidget(box);
        delete box;
    }
}

