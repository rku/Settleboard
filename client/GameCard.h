
#ifndef GAMECARD_H
#define GAMECARD_H 1

#include <QString>
#include <QtDebug>

#include "GamePixmap.h"
#include "FileManager.h"

class GameCard
{
    public:
        GameCard(const GameCard &c)
            : type(c.type), name(c.name), description(c.description),
            pixmap(c.pixmap), playRule(c.playRule), canPlayRule(c.canPlayRule),
            isSecret(c.isSecret) {}

        GameCard(QString t, QString n, QString d, QString i = QString(),
            QString p = QString(), QString c = QString(), bool s = true)
            : type(t), name(n), description(d), pixmap(GamePixmap(i)),
            playRule(p), canPlayRule(c), isSecret(s) {}

        const QString &getType() { return type; }
        const QString &getName() { return name; }
        const QString &getDescription() { return description; }
        const GamePixmap &getPixmap() { return pixmap; }
        const QString &getPlayRule() { return playRule; }
        const QString &getCanPlayRule() { return canPlayRule; }
        bool getIsSecret() { return isSecret; }

        static GamePixmap &getCoverPixmap()
        {
            static GamePixmap coverPixmap = GamePixmap("CardCover.png");
            return coverPixmap;
        }

    private:
        QString type;
        QString name;
        QString description;
        GamePixmap pixmap;
        QString playRule;
        QString canPlayRule;
        bool isSecret;
};

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
    "CardDevelopmentKnight.png", "ruleUserActionPlayKnightCard",\
    "ruleCanPlayKnightCard", false)

#define GAMECARD_BUILD_ROAD GameCard(\
    "Development",\
    "Build Road",\
    "Playing this card will allow you to place two roads for free.",\
    "CardDevelopmentBuildRoad.png", "ruleUserActionPlayBuildRoadCard",\
    "ruleCanPlayBuildRoadCard")

#define GAMECARD_MONOPOLY GameCard(\
    "Development",\
    "Monopoly",\
    "If you play this card, you can specify a resource and all players "\
    "who have this resource have to hand it over to you.",\
    "CardDevelopmentMonopoly.png", "ruleUserActionPlayMonopolyCard",\
    "ruleCanPlayMonopolyCard")

#define GAMECARD_INVENTION GameCard(\
    "Development",\
    "Invention",\
    "If you play this card, you will be able to select two resources of "\
    "your choice from the bank.",\
    "CardDevelopmentInvention.png", "ruleUserActionPlayInventionCard",\
    "ruleCanPlayInventionCard")

#define GAMECARD_WINNINGPOINT GameCard(\
    "Development",\
    "Winning Point",\
    "If you play this card, you will instantly get a winning point!",\
    "CardDevelopmentWinningPoint.png", "ruleUserActionPlayWinningPointCard",\
    "ruleCanPlayWinningPointCard")

#endif

