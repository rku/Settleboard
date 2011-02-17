
#ifndef GAMECARD_H
#define GAMECARD_H 1

#include <QString>

typedef struct _GameCard {
    _GameCard(const _GameCard &c)
        : type(c.type), name(c.name), description(c.description),
        iconFileName(c.iconFileName), playRule(c.playRule), isSecret(c.isSecret) {}
    _GameCard(QString t, QString n, QString d, QString i = QString(),
        QString p = QString(), bool s = true)
        : type(t), name(n), description(d), iconFileName(i), playRule(p), isSecret(s) {}

    QString type;
    QString name;
    QString description;
    QString iconFileName;
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
    GameCard("Resource", "Clay", "Clay Resource", "CardResourceClay")

// DEVELOPMENT CARDS

#define GAMECARD_KNIGHT GameCard( \
    "Development", "Knight", \
    "If you play this card, you can steal a card from another player.",\
    "CardKnight.png", "rulePlayKnightCard", false)

#define GAMECARD_BUILD_ROAD GameCard(\
    "Development",\
    "Build Road",\
    "Playing this card will allow you to place two roads for free.",\
    "CardBuildRoad.png", "rulePlayBuildRoadCard")

#define GAMECARD_MONOPOLY GameCard(\
    "Development",\
    "Monopoly",\
    "If you play this card, you can specify a resource and all players "\
    "who have this resource have to hand it to you.",\
    "CardMonopoly.png", "rulePlayMonopolyCard")

#define GAMECARD_INVENTION GameCard(\
    "Development",\
    "Invention",\
    "If you play this card, you will be able to select two resources of "\
    "your choice from the bank.",\
    "CardInvention.png", "rulePlayInventionCard")

#define GAMECARD_WINNINGPOINT GameCard(\
    "Development",\
    "Winning Point",\
    "If you play this card, you will instantly get one winning point!",\
    "CardWinningPoint.png", "rulePlayWinningPointCard")

#endif

