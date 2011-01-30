/**
 * RSETTLE
 *
 * This file is part of
 * RSETTLE - A board game like settlers.
 * Copyright (C) 2007 Rene Kuettner <rene@bitkanal.net>
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

#include "Board.h"
#include "Game.h"
#include "Crossroad.h"
#include "BoardState.h"
#include "FileManager.h"
#include "Roadway.h"

#include <math.h>

Board::Board(Game *_game) : GameObject(_game)
{
    isSelectionModeActive   = false;
    selectedObject          = NULL;
    state                   = BOARD_STATE_SELECT_CROSSROAD;
}

Board::~Board()
{
    freeObjects();
}

void Board::freeObjects()
{
    while(!numberChips.isEmpty()) delete numberChips.takeFirst();
    while(! boardTiles.isEmpty()) delete boardTiles.takeFirst();
    while(! crossroads.isEmpty()) delete crossroads.takeFirst();
    while(!   roadways.isEmpty()) delete roadways.takeFirst();
}

void Board::render()
{
    for(int i = 0; i < boardTiles.size(); ++i)
        boardTiles.at(i)->draw();

    for(int i = 0; i < crossroads.size(); ++i)
        crossroads.at(i)->draw();

    for(int i = 0; i < roadways.size(); ++i)
        roadways.at(i)->draw();

    GLGameModel *robber = new GLGameModel(game);
    robber->load(FileManager::getPathOfResource("Objects", "Robber", "obj"));
    robber->setColor(Qt::black);
    robber->setScale(0.7);
    robber->draw();
    delete robber;
}

void Board::update()
{
    game->getGLWidget()->updateGL();
}

/* returns the glgamemodel objects in objs at 2d mouse position pos*/
template <typename T>
const T Board::getObjectsAtMousePos(T objs, const QPoint &pos)
{
    QList<GLuint> hits;
    GLWidget *widget = game->getGLWidget();

    widget->beginGLSelection(pos);

    for(int i = 0; i < objs.size(); i++)
    {
        glLoadName(i);
        objs.at(i)->draw();
    }

    hits = widget->endGLSelection();

    T result;
    for(int i = 0; i < hits.size(); i++) result.append(objs.at(hits.at(i)));
    return result;
}

template <typename T>
void Board::highlightObjectsAtMousePos(T objs, const QPoint &pos)
{
    T selObjs;

    for(int i = 0; i < objs.size(); i++)
    {
        objs.at(i)->setIsHighlighted(false);
        if(objs.at(i)->getIsSelectable()) selObjs.append(objs.at(i));
    }

    T hits = getObjectsAtMousePos(selObjs, pos);
    if(hits.size())
        for(int i = 0; i < hits.size(); i++)
            hits.at(i)->setIsHighlighted(true);
}

GLGameModel *Board::getSelectableObjectAtMousePos(const QPoint &pos)
{
    QList<GLGameModel*> selObjs, hits;
    int max = 1;

#define ADD_SELECTED_OBJECTS(a) if(a.size()>i) {\
        GLGameModel *obj = dynamic_cast<GLGameModel*>(a.at(i)); \
        if(obj->getIsSelectable()) selObjs.append(obj); \
        done = false; }

    for(int i = 0; i < max; i++)
    {
        bool done = true;

        ADD_SELECTED_OBJECTS(crossroads);
        ADD_SELECTED_OBJECTS(roadways);
        ADD_SELECTED_OBJECTS(boardTiles);

        if(!done) max++;
    }

    hits = getObjectsAtMousePos(selObjs, pos);
    return (hits.size()>0) ? hits.at(0) : NULL;
}

bool Board::handleMouseClick(const QPoint &mousePos)
{
    if(getIsSelectionModeActive())
    {
        GLGameModel *obj = getSelectableObjectAtMousePos(mousePos);
        setSelectedObject(obj);
        endSelectionMode();
        return true;
    }

    // execute test rule
    for(int i = 20; i < 35; i+=4)
    {
        selectedObject = crossroads.at(i);
        game->getRules()->executeRule("ruleCrossroadSelected", game->getPlayers()[0]);
        game->getRules()->executeRule("ruleBuildSettlement", game->getPlayers()[0]);
        selectedObject = NULL;
    }
    game->getRules()->executeRule("ruleUserActionBuildCity", game->getPlayers()[0]);
    return true;
}

bool Board::handleMouseOver(const QPoint &mousePos)
{
    // highlight selectable objects at mousepos
    highlightObjectsAtMousePos(crossroads, mousePos);
    highlightObjectsAtMousePos(roadways, mousePos);
    highlightObjectsAtMousePos(boardTiles, mousePos);

    game->getGLWidget()->updateGL();

    return false;
}

void Board::setSelectionMode()
{
    Q_ASSERT(!getIsSelectionModeActive());
    Q_ASSERT(selectedObject == NULL);

    int max = 1;

    // disable all board objects except the selectable ones
    for(int i = 0; i < max; i++)
    {
        bool done = true;

#define DISABLE_NONSELECTABLE_OBJECT_OF(a) \
    if(a.size() > i) { \
        a.at(i)->setIsEnabled(a.at(i)->getIsSelectable()); done = false; }

        DISABLE_NONSELECTABLE_OBJECT_OF(crossroads);
        DISABLE_NONSELECTABLE_OBJECT_OF(roadways);
        DISABLE_NONSELECTABLE_OBJECT_OF(boardTiles);

        if(!done) max++;
    }

    selectedObject = NULL;
    isSelectionModeActive = true;
}

void Board::endSelectionMode()
{
    Q_ASSERT(getIsSelectionModeActive());

    isSelectionModeActive = false;
    resetBoardState();

    if(game->getRules()->getIsRuleChainWaiting())
    {
        game->getRules()->continueRuleChain();
    }

    selectedObject = NULL;
}

void Board::setSelectedObject(GLGameModel *gm)
{
    selectedObject = gm;
}

void Board::resetBoardState(BoardObjectState s)
{
    BoardState state;

#define CREATE_RESET_STATE_FOR(objs, sto) if(1) { \
    for(int i = 0; i < objs.size(); i++) { \
        s.index = i; \
        state.sto.append(s); } }

    CREATE_RESET_STATE_FOR(boardTiles, tileStates);
    CREATE_RESET_STATE_FOR(crossroads, crossroadStates);
    CREATE_RESET_STATE_FOR(roadways, roadwayStates);

    updateBoardState(state);
}

void Board::updateBoardState(BoardState &newState)
{
    int max = 1;
    BoardObjectState s;

    for(int i = 0; i < max; i++)
    {
        bool done = true;

#define UPDATE_GLOBJECT_STATE(obj, state) if(1) { \
    obj->setIsSelectable(s.selectable); \
    obj->setIsEnabled(s.enabled); \
    done = false; }

        if(newState.tileStates.size() >= max)
        {
            s = newState.tileStates.at(i);
            HexTile *h = boardTiles.at(s.index);
            UPDATE_GLOBJECT_STATE(h, s);
        }

        if(newState.roadwayStates.size() >= max)
        {
            s = newState.roadwayStates.at(i);
            Roadway *r = roadways.at(s.index);
            UPDATE_GLOBJECT_STATE(r, s);
        }

        if(newState.crossroadStates.size() >= max)
        {
            s = newState.crossroadStates.at(i);
            Crossroad *c = crossroads.at(s.index);
            UPDATE_GLOBJECT_STATE(c, s);
        }

        if(!done) max++;
    }

    game->getGLWidget()->updateGL();
}

// load a board from a plain textfile
// See MAPFORMAT for a description of the
// file format.
bool Board::loadFromFile(const QString& filename)
{
    QFile boardFile(filename);
    QTextStream stream(&boardFile);
    char *pDir = tileData;

    memset(pDir, 0, BOARD_MAX_TILES);

    if(!boardFile.open(QFile::ReadOnly))
    {
        qCritical() << "Could'nt load board file: " << filename;
        return false;
    }

    name   = stream.readLine();
    author = stream.readLine();

    stream >> width;
    stream >> height;

    if((width * height) > (BOARD_MAX_TILES-1))
    {
        qCritical() << "Board too large.";
        return false;
    }

    if(width < 2 || height < 2)
    {
        qCritical() << "Boardsize must be at least 2x2.";
        return false;
    }

    for(unsigned int i = 0; i < (width * height); i++)
    {
        if(stream.atEnd())
        {
            qCritical() << "Can't load board. Invalid data size.";
            return false;
        }

        stream >> *pDir;

        if(*pDir == '\n' || *pDir == ' ')
            { i--; } else { pDir++; }
    }

    qDebug() << "Board loaded: " << filename;

    boardFile.close();

    return true;
}

bool Board::loadByName(const QString &name)
{
    return loadFromFile(FileManager::getPathOfResource("Maps", name, "rsm"));
}

QVector3D Board::getPosForTile(HexTile *tile, int col, int row)
{
    float w = tile->getWidth();
    float d = tile->getDepth() - 0.6f;

    // center - boardwidth/2 + tilewidth*col
    return QVector3D(
        0 - ( width * (w/2) ) + (col * w) + (row % 2) * (w/2),
        0.0f,
        // center - boardheight/2 - tiledepth/2 + row*tiledepth
        (d/2) - ( height * (d/2) ) + (row * d));
}

void Board::generate()
{
    int row = 0, col = -1;
    QVector3D pos;

    // discard old board
    freeObjects();

    for(char *p = tileData; row < (int)height && *p; p++)
    {
        HexTile *newTile = new HexTile(game);

        col++;
        if(col >= (int)width)
        {
            col = 0;
            row++;
        }

        switch(*p)
        {
            case 'X':
                newTile->setFixedPosition(true);
            case 'x':
                newTile->setType(HEXTILE_TYPE_WATER);
                break;
            case 'E': // weed
                newTile->setFixedPosition(true);
            case 'e':
                newTile->setType(HEXTILE_TYPE_WEED);
                break;
            case 'S': // sheep
                newTile->setFixedPosition(true);
            case 's':
                newTile->setType(HEXTILE_TYPE_SHEEP);
                break;
            case 'W': // wood
                newTile->setFixedPosition(true);
            case 'w':
                newTile->setType(HEXTILE_TYPE_WOOD);
                break;
            case 'C': // clay
                newTile->setFixedPosition(true);
            case 'c':
                newTile->setType(HEXTILE_TYPE_CLAY);
                break;
            case 'G': // gold
                newTile->setFixedPosition(true);
            case 'g':
                newTile->setType(HEXTILE_TYPE_GOLD);
                break;
            case 'O': // ore
                newTile->setFixedPosition(true);
            case 'o':
                newTile->setType(HEXTILE_TYPE_ORE);
                break;
            case 'D': // desert
                newTile->setType(HEXTILE_TYPE_DESERT);
                break;
            default:
                continue;
        }

        // set position
        pos = getPosForTile(newTile, col, row);
        newTile->setPos(pos);

        // setup crossroads and roadways
        QList<QVector3D> vertices = newTile->getCornerVertices();
        Q_ASSERT(vertices.size() == 6);
        vertices.append(vertices[0]);
        Crossroad *lastCrossroad = NULL;
        for(int i = 1; i < vertices.size(); i++)
        {
            Crossroad *cr = getCrossroadNearPosition(vertices.at(i-1), true);
            cr->addTile(newTile);

            if(lastCrossroad != NULL) cr->addNeighbour(lastCrossroad);

            Roadway *rw = getRoadwayNear(vertices.at(i-1), vertices.at(i), true);
            rw->addTile(newTile);
            rw->addCrossroad(cr);

            lastCrossroad = cr;
        }

        // add tile
        boardTiles.insert(boardTiles.begin(), newTile);
    }

    qDebug() << "Board generated:" << crossroads.size() << "crossroads" <<
        "," << roadways.size() << "roadways" <<
        "and" << boardTiles.size() << "tiles";
}

Crossroad *Board::getCrossroadNearPosition(QVector3D pos, bool create)
{
    for(int i = 0; i < crossroads.size(); i++)
    {
        Crossroad *a = crossroads.at(i);
        QVector3D v = a->getVertex();

        if(qAbs((v - pos).length()) < 0.1) return a;
    }

    // not found
    Crossroad *c = NULL;

    if(create)
    {
        c = new Crossroad(game, pos);
        crossroads.append(c);
    }

    return c;
}

Roadway *Board::getRoadwayNear(QVector3D a, QVector3D b, bool create)
{
    for(int i = 0; i < roadways.size(); i++)
    {
        Roadway *r = roadways.at(i);
        QVector3D ra = r->getVertexA();
        QVector3D rb = r->getVertexB();

#define COMP_ROADWAY_VECS(c,d) if(1) { \
    if(qAbs((a - c).length()) + \
        qAbs((b - d).length()) < 0.2) return r; }

        COMP_ROADWAY_VECS(ra, rb);
        COMP_ROADWAY_VECS(rb, ra);
    }

    Roadway *newR = NULL;

    if(create)
    {
        newR = new Roadway(game, a, b);
        roadways.append(newR);
    }

    return newR;
}

