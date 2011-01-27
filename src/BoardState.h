
#ifndef BOARDSTATE_H
#define BOARDSTATE_H 1

#include <QString>
#include <QList>

typedef struct _BoardObjectState {
    unsigned int index;
    unsigned int playerIndex;
    bool selectable;
    bool visible;
    bool enabled;
    bool removed;
} BoardObjectState;

class BoardState
{
public:
    QString logMessage;
    QList<BoardObjectState> crossroadStates;
    QList<BoardObjectState> roadwayStates;
    QList<BoardObjectState> tileStates;
    QList<BoardObjectState> buildingStates;
};

#endif

