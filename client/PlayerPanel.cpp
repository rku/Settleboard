
#include "Player.h"
#include "FileManager.h"
#include "GamePixmap.h"
#include "PlayerPanel.h"

PlayerPanel::PlayerPanel(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    boxWidth = 160;
    columns = 2;
    QHBoxLayout *lt = new QHBoxLayout();
    QWidget *widget = new QWidget(this);

    widget->setLayout(lt);
    setWidget(widget);

    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setTitleBarWidget(new QWidget(this));

    clear();
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
        box->setFixedWidth(boxWidth);

        QHBoxLayout *l = (QHBoxLayout*)widget()->layout();
        QGridLayout *innerL = new QGridLayout();
        //Q_ASSERT(l->count() > 0); // we expect at least a stretcher
        l->insertWidget(l->count(), box);
        innerL->setVerticalSpacing(10);
        box->setLayout(innerL);
        playerBoxes.insert(player, box);
        setFixedWidth((boxWidth + 20)*playerBoxes.count());
    }
    else box = playerBoxes.value(player);

    QString style = QString("::title { color: %1; }");
    QColor color = (player->getIsDisconnected()) ? 
        Qt::gray : player->getColor().lighter(130);
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
    int row = 0;
    int col = 0;
    while(gl->itemAtPosition(row, col) != 0)
    {
        col++;
        if(col >= (int)(columns * 2))
        {
            row++;
            col = 0;
        }
    }

    // prepare icon
    GamePixmap icon = GamePixmap(iconFile);
    if(usePlayerColor) icon.setOverlayColor(player->getColor());
    icon.scale(QSize(16,16));

    // add new row for the registered info value
    QLabel *iconLabel = new QLabel(box);
    if(!icon.isNull()) iconLabel->setPixmap(icon);
    iconLabel->setToolTip(description);
    QLabel *valueLabel = new QLabel("0", box);
    valueLabel->setToolTip(description);

    gl->addWidget(iconLabel,  row, col, Qt::AlignRight);
    gl->addWidget(valueLabel, row, col + 1, Qt::AlignLeft);

    playerInfos.insertMulti(player, infoName);

    gl->setRowStretch(row, 0);
    gl->setRowStretch(row + 1, 1);
}

void PlayerPanel::updatePlayerInfo(Player *player, const QString infoName, int value)
{
    QGroupBox *box = getPlayerBox(player);
    QGridLayout *gl = (QGridLayout*)box->layout();

    // insert objects info
    Q_ASSERT(playerInfos.contains(player));
    Q_ASSERT(playerInfos.values(player).contains(infoName));

    // the list of values is adjacently ordered
    QList<QString> values = playerInfos.values(player);
    int index = values.size() - values.indexOf(infoName) - 1;
    int row = qRound(index / columns);
    int col = qRound(index % columns) * 2 + 1;
    Q_ASSERT(gl->itemAtPosition(row, col) != NULL);
    QLabel *label = (QLabel*)gl->itemAtPosition(row, col)->widget();
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

    setFixedWidth(10);
}

