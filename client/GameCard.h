
#ifndef GAMECARD_H
#define GAMECARD_H 1

#include <QString>

typedef struct _GameCard {
    _GameCard(const _GameCard &c)
        : type(c.type), name(c.name), description(c.description),
        playRule(c.playRule) {}
    _GameCard(QString t, QString n, QString d, QString p = QString())
        : type(t), name(n), description(d), playRule(p) {}

    QString type;
    QString name;
    QString description;
    QString playRule;
} GameCard;

#endif

