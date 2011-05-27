
#ifndef GAMECARD_PANEL_H
#define GAMECARD_PANEL_H 1

#include <QtGui>

class GameCard;

class GameCardPanel : public QDockWidget
{
    Q_OBJECT

    public:
        GameCardPanel(const QString &title, QWidget *parent = 0);
        ~GameCardPanel();

        void clear();
        void addCard(GameCard *card);

    protected slots:
        void updateButtonState();
        void showCardInfo();
        void playCard();

    private:
        QList<GameCard*> cards;
        QPushButton *buttonInfo;
        QPushButton *buttonPlay;
        QListWidget *cardList;
};

#endif

