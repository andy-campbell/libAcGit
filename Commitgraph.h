#ifndef COMMITGRAPH_H
#define COMMITGRAPH_H

#include <QList>
#include <QSet>
#include <QVector>

#include "Commit.h"
#include "Graph.h"
#include "Sha.h"


namespace AcGit
{

class CommitGraph
{
public:
    CommitGraph(QList<Commit *> *CommitsList);
private:
    bool branchMergeAppend(QVector<Graph::GraphSymbol> *currentRowState, Graph::GraphSymbol type, int &numHandled, int size, bool handledLast);
    void handleRowTypeNotEqual(QVector<Graph::GraphSymbol> *currentRowState, bool emptyCommit, bool handledLast);
    bool appendRowTypeEqual(QVector<Graph::GraphSymbol> *currentRowState, Graph::GraphSymbol type, int &numHandled, int size);
    void branchsExtras(QVector<Graph::GraphSymbol> *currentRowState, int &prevMaxRows, QSet<int> &branchedToRow, Graph::GraphSymbol activeRowType, int handledBranches);
    QVector<Graph::GraphSymbol> *buildUpLane(QSet<int> branchedToRow, QSet<int> mergeFromRow, QVector<Sha> &nextCommits, int activeRow, Graph::GraphSymbol activeRowType, int curMaxRows, int prevMaxRow);
    Graph *populateCommit(AcGit::Commit &commit, QVector<Sha> &nextCommits, Graph *prevCommit);
};

}

#endif // COMMITGRAPH_H
