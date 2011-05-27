
#ifndef GAMECARD_PANEL_H
#define GAMECARD_PANEL_H 1

#include <QtGui>

class GameCardPanel : public QDockWidget
{
    Q_OBJECT

    public:
        GameCardPanel(const QString &title, QWidget *parent = 0);
        ~GameCardPanel();

    protected:

    private:
        QPushButton *buttonInfo;
        QPushButton *buttonPlay;
        QListWidget *cardList;
};

#endif

