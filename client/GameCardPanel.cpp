
#include "GameCard.h"
#include "GameCardPanel.h"

GameCardPanel::GameCardPanel(const QString &title, QWidget *parent)
    : QDockWidget(parent)
{
    // setup ui
    QGridLayout *l = new QGridLayout();
    QWidget *widget = new QWidget(this);

    QLabel *caption = new QLabel("Development Cards:", this);
    l->addWidget(caption, 0, 0, 1, 2);

    cardList = new QListWidget(this);
    l->addWidget(cardList, 1, 0, 1, 2);
    connect(cardList, SIGNAL(itemSelectionChanged()), this,
        SLOT(updateButtonState()));

    buttonInfo = new QPushButton("Info", this);
    buttonInfo->setEnabled(false);
    l->addWidget(buttonInfo, 2, 0);
    connect(buttonInfo, SIGNAL(clicked()), this, SLOT(showCardInfo()));

    buttonPlay = new QPushButton("Play", this);
    buttonPlay->setEnabled(false);
    l->addWidget(buttonPlay, 2, 1);
    connect(buttonPlay, SIGNAL(clicked()), this, SLOT(playCard()));

    l->setColumnStretch(2, 1);
    l->setRowStretch(3, 1);
    widget->setLayout(l);
    setWidget(widget);

    setTitleBarWidget(new QWidget(this));
}

GameCardPanel::~GameCardPanel()
{
}

void GameCardPanel::updateButtonState()
{
    QList<QListWidgetItem*> items = cardList->selectedItems();

    buttonInfo->setEnabled(items.count() > 0);
}

void GameCardPanel::showCardInfo()
{
    QDialog *infoDialog = new QDialog(this);
    QList<QListWidgetItem*> items = cardList->selectedItems();

    Q_ASSERT(items.count() > 0);
    QListWidgetItem *item = items.at(0);
    int index = qvariant_cast<int>(item->data(Qt::UserRole));
    GameCard *card = cards.at(index);

    infoDialog->setModal(true);
    infoDialog->setFixedSize(QSize(300, 200));

    QGridLayout *layout = new QGridLayout();

    QTextEdit *textView = new QTextEdit(infoDialog);
    textView->setText(card->getDescription());
    textView->setReadOnly(true);
    layout->addWidget(textView, 0, 0);

    QPushButton *buttonHide = new QPushButton("Hide", infoDialog);
    layout->addWidget(buttonHide, 1, 0, Qt::AlignCenter);
    connect(buttonHide, SIGNAL(clicked()), infoDialog, SLOT(accept()));

    infoDialog->setLayout(layout);
    infoDialog->exec();

    delete infoDialog;
}

void GameCardPanel::playCard()
{
}

void GameCardPanel::clear()
{
    // clear list and cards
    cardList->clear();
    cards.clear();
}

void GameCardPanel::addCard(GameCard *card)
{
    Q_ASSERT(card->getType() == "Development");

    QListWidgetItem *item = new QListWidgetItem(cardList);
    item->setText(card->getName());

    // also save a pointer to the GameCard for each list entry
    // in order to be able to access it easily
    item->setData(Qt::UserRole, qVariantFromValue((int)cards.count()));
    cards.append(card);
}

