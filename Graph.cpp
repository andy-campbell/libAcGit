#include "Graph.h"

namespace AcGit
{

Graph::Graph(int activeRowIndex, QVector<enum GraphSymbol> *laneState, int totalRows, enum GraphSymbol type)
{
    activeRow = activeRowIndex;
    lane = laneState;
    this->totalRows = totalRows;
    this->type = type;
}

int Graph::activeRowNumber()
{
    return activeRow;
}

int Graph::getTotalRows()
{
    return totalRows;
}

QVector<Graph::GraphSymbol> *Graph::rowState()
{
    return lane;
}

Graph::GraphSymbol Graph::typeOfRow()
{
    return type;
}

}
