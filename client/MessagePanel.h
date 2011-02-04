
#ifndef MESSAGE_PANEL_H
#define MESSAGE_PANEL_H 1

#include <QtGui>

class Player;

class MessagePanel : public QWidget
{
    Q_OBJECT

    public:
        MessagePanel(QWidget *parent);
        ~MessagePanel();

        void addSystemMessage(const QString msg);
        void addLogMessage(Player*, const QString msg);
        void addChatMessage(Player*, const QString msg);

    protected:
        QLineEdit *input;
        QTextEdit *output;
};

#endif

