#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>

namespace AcGit {



class Graph
{
public:
        enum GraphSymbol
        {
            NO_COMMIT = 0,
            NO_COMMIT_H,
            NO_COMMIT_WORKING_DIR,
            EMPTY_LANE,
            EMPTY_LANE_H,
            NORMAL_COMMIT,
            NORMAL_COMMIT_H,
            BRANCH_COMMIT,
            BRANCH_COMMIT_UP,
            BRANCH_COMMIT_UP_H,
            MERGE_COMMIT,
            MERGE_COMMIT_DOWN,
            MERGE_COMMIT_DOWN_H,
            BRANCH_MERGE_COMMIT,
            BRANCH_MERGE_COMMIT_DOWN,
            BRANCH_MERGE_COMMIT_DOWN_H,
            BRANCH_MERGE_COMMIT_UP,
            BRANCH_MERGE_COMMIT_UP_H,
            BRANCH_MERGE_COMMIT_BOTH,
            BRANCH_MERGE_COMMIT_BOTH_H
        };

        Graph(int activeRowIndex, QVector<enum GraphSymbol> *laneState, int totalRows, GraphSymbol type);

        int activeRowNumber();
        int getTotalRows ();
        QVector<enum GraphSymbol>* rowState();
        enum GraphSymbol typeOfRow();
private:
        int activeRow;
        enum GraphSymbol type;
        QVector<enum GraphSymbol> *lane;
        int totalRows;
};

}
#endif // GRAPH_H
