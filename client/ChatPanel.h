
#ifndef CHAT_PANEL_H
#define CHAT_PANEL_H 1

#include <QtGui>

class ChatPanel : public QWidget
{
    Q_OBJECT

    public:
        ChatPanel(QWidget *parent);
        ~ChatPanel();

    protected:
        QLineEdit *input;
        QTextEdit *output;
};

#endif

