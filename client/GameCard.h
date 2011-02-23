
#ifndef GAMECARD_H
#define GAMECARD_H 1

#include <QString>

#include "GamePixmap.h"

typedef struct _GameCard {
    _GameCard(const _GameCard &c)
        : type(c.type), name(c.name), description(c.description),
        pixmap(c.pixmap), playRule(c.playRule), isSecret(c.isSecret) {}
    _GameCard(QString t, QString n, QString d, QString i = QString(),
        QString p = QString(), bool s = true)
        : type(t), name(n), description(d), pixmap(GamePixmap(i)),
            playRule(p), isSecret(s) {}

    QString type;
    QString name;
    QString description;
    GamePixmap pixmap;
    QString playRule;
    bool isSecret;
} GameCard;

// all available game cards are defined here at the moment

// RESOURCES

#define GAMECARD_WHEAT \
    GameCard("Resource", "Wheat", "Wheat Resource", "CardResourceWheat.png")
#define GAMECARD_ORE \
    GameCard("Resource", "Ore", "Ore Resource", "CardResourceOre.png")
#define GAMECARD_LUMBER \
    GameCard("Resource", "Lumber", "Lumber Resource", "CardResourceLumber.png")
#define GAMECARD_SHEEP \
    GameCard("Resource", "Wool", "Wool Resource", "CardResourceWool.png")
#define GAMECARD_CLAY \
    GameCard("Resource", "Clay", "Clay Resource", "CardResourceClay.png")

// DEVELOPMENT CARDS

#define GAMECARD_KNIGHT GameCard( \
    "Development", "Knight", \
    "If you play this card, you can steal a card from another player.",\
    "CardDevelopmentKnight.png", "rulePlayKnightCard", false)

#define GAMECARD_BUILD_ROAD GameCard(\
    "Development",\
    "Build Road",\
    "Playing this card will allow you to place two roads for free.",\
    "CardDevelopmentBuildRoad.png", "rulePlayBuildRoadCard")

#define GAMECARD_MONOPOLY GameCard(\
    "Development",\
    "Monopoly",\
    "If you play this card, you can specify a resource and all players "\
    "who have this resource have to hand it over to you.",\
    "CardDevelopmentMonopoly.png", "rulePlayMonopolyCard")

#define GAMECARD_INVENTION GameCard(\
    "Development",\
    "Invention",\
    "If you play this card, you will be able to select two resources of "\
    "your choice from the bank.",\
    "CardDevelopmentInvention.png", "rulePlayInventionCard")

#define GAMECARD_WINNINGPOINT GameCard(\
    "Development",\
    "Winning Point",\
    "If you play this card, you will instantly get a winning point!",\
    "CardDevelopmentWinningPoint.png", "rulePlayWinningPointCard")

#endif

