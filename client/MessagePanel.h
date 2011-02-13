
#ifndef MESSAGE_PANEL_H
#define MESSAGE_PANEL_H 1

#include <QtGui>

class Player;

class MessagePanel : public QDockWidget
{
    Q_OBJECT

    public:
        MessagePanel(const QString &title, QWidget *parent = 0);
        ~MessagePanel();

        void addSystemMessage(const QString msg);
        void addLogMessage(Player*, const QString msg);
        void addChatMessage(Player*, const QString msg);

    protected slots:
        void newMessageAvailable();

    protected:
        QLineEdit *input;
        QTextEdit *output;
};

#endif

