/**
 * Settleboard3D
 *
 * This file is part of
 * Settleboard3D - A board game like settlers.
 * Copyright (C) 2010, 2011 Rene Kuettner <rene@bitkanal.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameCard.h" 

#include "FileManager.h"

GameCard::GameCard(const GameCard &c)
 : type(c.type),
   name(c.name),
   description(c.description),
   pixmap(c.pixmap),
   playRule(c.playRule),
   canPlayRule(c.canPlayRule),
   isSecret(c.isSecret)
{
}

GameCard::GameCard(QString _type,
                   QString _name,
                   QString _description,
                   QString _pixmapFilename,
                   QString _playRule,
                   QString _canPlayRule,
                   bool _isSecret)
 : type(_type),
   name(_name),
   description(_description),
   pixmap(GamePixmap(_pixmapFilename)),
   playRule(_playRule),
   canPlayRule(_canPlayRule),
   isSecret(_isSecret)
{
}

GamePixmap& GameCard::getCoverPixmap()
{
    static GamePixmap coverPixmap = GamePixmap("CardCover.png");
    return coverPixmap;
}

// RESOURCES

GameCard GameCard::createResourceCardWheat()
{
    return GameCard("Resource",
                    "Wheat",
                    "Wheat Resource",
                    "CardResourceWheat.png");
}

GameCard GameCard::createResourceCardOre()
{
    return GameCard("Resource",
                    "Ore",
                    "Ore Resource",
                    "CardResourceOre.png");
}

GameCard GameCard::createResourceCardLumber()
{
    return GameCard("Resource",
                    "Lumber",
                    "Lumber Resource",
                    "CardResourceLumber.png");
}

GameCard GameCard::createResourceCardWool()
{
    return GameCard("Resource",
                    "Wool",
                    "Wool Resource",
                    "CardResourceWool.png");
}

GameCard GameCard::createResourceCardClay()
{
    return GameCard("Resource",
                    "Clay",
                    "Clay Resource",
                    "CardResourceClay.png");
}

// DEVELOPMENT CARDS

GameCard GameCard::createDevelopmentCardKnight()
{
    return GameCard("Development",
                    "Knight",
                    "If you play this card, you can steal a card from another player.",
                    "CardDevelopmentKnight.png",
                    "ruleUserActionPlayKnightCard",
                    "ruleCanPlayKnightCard",
                    false);
}

GameCard GameCard::createDevelopmentCardBuildRoad()
{
    return GameCard("Development",
                    "Build Road",
                    "Playing this card will allow you to place two roads for free.",
                    "CardDevelopmentBuildRoad.png",
                    "ruleUserActionPlayBuildRoadCard",
                    "ruleCanPlayBuildRoadCard");
}

GameCard GameCard::createDevelopmentCardMonopoly()
{
    return GameCard("Development",
                    "Monopoly",
                    "If you play this card, you can specify a resource and "
                    "all players who have this resource have to hand it over "
                    "to you.",
                    "CardDevelopmentMonopoly.png",
                    "ruleUserActionPlayMonopolyCard",
                    "ruleCanPlayMonopolyCard");
}

GameCard GameCard::createDevelopmentCardInvention()
{
    return GameCard("Development",
                    "Invention",
                    "If you play this card, you will be able to select two "
                    "resources of your choice from the bank.",
                    "CardDevelopmentInvention.png",
                    "ruleUserActionPlayInventionCard",
                    "ruleCanPlayInventionCard");
}

GameCard GameCard::createDevelopmentCardWinningPoint()
{
    return GameCard("Development",
                    "Winning Point",
                    "If you play this card, you will instantly get a winning point!",
                    "CardDevelopmentWinningPoint.png",
                    "ruleUserActionPlayWinningPointCard",
                    "ruleCanPlayWinningPointCard");
}

