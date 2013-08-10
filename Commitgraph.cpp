#include <QDebug>

#include "Commitgraph.h"
namespace AcGit {


CommitGraph::CommitGraph(QList<Commit *> *CommitsList)
{

    QVector<Sha> nextCommits;
    nextCommits.reserve(400);
    nextCommits.insert(0, *CommitsList->at(0));

    Graph *prevCommit = nullptr;
    Graph *commitGraph;
    foreach(Commit* commit, *CommitsList)
    {
        commitGraph = populateCommit(*commit, nextCommits, prevCommit);
        commit->addGraph(commitGraph);
        prevCommit = commitGraph;
    }
}





/**
 * @brief branchMergeAppend This function is used to add branch merge up and
 * branch merge down or their _H varients to currentRowState.
 *
 * @param currentRowState The current state of the commits lanes
 * @param type The base type which is to add be added to currentRowState
 * @param numHandled The number of merges or branches which have been handled
 * @param size The total number of merges or branches which can be handled.
 * @param handledLast Dependant on if the last merge has been added to currrentRowState and
 * and this is called from the branch check and vise versa for the branch case.
 * @return true or false depending on if is the last of the type to be handled.
 */
bool CommitGraph::branchMergeAppend (QVector<enum Graph::GraphSymbol> *currentRowState, enum Graph::GraphSymbol type,
                        int &numHandled, int size, bool handledLast)
{
    numHandled++;
    if (handledLast && numHandled == size)
    {
        currentRowState->append(type);
        return true;
    }
    else
    {
        currentRowState->append(static_cast<Graph::GraphSymbol> (type + 1));
    }
    return false;

}

/**
 * @brief handleRowTypeNotEqual This function adds an empty lanes or no commit lane
 * to currentRowState. This is called as part of the dealing with merge commits or
 * branch commits.
 *
 * @param currentRowState The current state of the commits lanes
 * @param emptyCommit This is to be true if and empty lane is to be displayed and
 * false if no commit is to be displayed.
 * @param handledLast True if the last merge or branch has been handled.
 */
void CommitGraph::handleRowTypeNotEqual (QVector<Graph::GraphSymbol> *currentRowState, bool emptyCommit, bool handledLast)
{
    if (emptyCommit)
    {
        if (handledLast)
            currentRowState->append(Graph::EMPTY_LANE);
        else
            currentRowState->append(Graph::EMPTY_LANE_H);
    }
    else
    {
        if (handledLast)
            currentRowState->append(Graph::NO_COMMIT);
        else
            currentRowState->append(Graph::NO_COMMIT_H);
    }
}

/**
 * @brief appendRowTypeEqual Adds type or type + 1 which is the _H version of type to
 * currentRow State. This function returns true when it is the last of the type to
 * be handled.
 *
 * @param currentRowState The current state of the commits lanes
 * @param type The base type which is to add be added to currentRowState
 * @param numHandled The number of merges or branches which have been handled
 * @param size The total number of merges or branches which can be handled.
 * @return true or false depending on if is the last of the type to be handled.
 */
bool CommitGraph::appendRowTypeEqual(QVector<enum Graph::GraphSymbol> *currentRowState, Graph::GraphSymbol type,
                        int &numHandled, int size)
{
    numHandled++;
    if (numHandled == size)
    {
        currentRowState->append(type);
        return true;
    }
    else
    {
        // cast type back to an enum of type + 1 which is the _H version of the type given
        currentRowState->append(static_cast<Graph::GraphSymbol> (type + 1));
    }

    return false;
}

/**
 * @brief branchsExtras This function handles the case were the previous commit had more
 * lanes than is currently in the row state. This occurs when there are branches or multiple
 * branches from this commit.
 *
 * @param currentRowState The current state of the commits lanes
 * @param prevMaxRows The number of rows the previous commit contains
 * @param branchedToRow A Set that contains all of the indexs of the branches.
 * @param activeRowType The type of the active row
 * @param handledBranches The number of branches which have been added to the currentRowState
 */
void CommitGraph::branchsExtras (QVector<enum Graph::GraphSymbol> *currentRowState, int &prevMaxRows, QSet<int> &branchedToRow,
                    Graph::GraphSymbol activeRowType, int handledBranches)
{
    if (currentRowState->size() < prevMaxRows)
    {
        int diff = prevMaxRows - currentRowState->size();
        int numRows = currentRowState->size();

        for (int i = 0; i < diff; i++)
        {
            if (branchedToRow.contains(numRows + i))
            {
                if (activeRowType == Graph::BRANCH_MERGE_COMMIT)
                {
                    appendRowTypeEqual (currentRowState, Graph::BRANCH_MERGE_COMMIT_UP,
                                                            handledBranches, branchedToRow.size());
                }
                else if (activeRowType == Graph::BRANCH_COMMIT)
                {
                    appendRowTypeEqual (currentRowState, Graph::BRANCH_COMMIT_UP,
                                                            handledBranches, branchedToRow.size());
                }
            }
            else
            {
                currentRowState->append(Graph::EMPTY_LANE_H);
            }
        }
    }
}


/**
 * @brief buildUpLane This function builds up a QVector with for a specific commit containing what should
 * be displayed in the Rev view column.
 *
 * @param branchedToRow This is a set of branches that the commit contains. This information is used when the
 * active row type is Branch_Commit or Merge_Branch_Commit.
 * @param mergeFromRow This is a set of merges that the commit contains. This information is used when the
 * active row type Merge_Commit or Merge_Branch_Commit
 * @param nextCommits - A Vector containing a list of the next oid to match against
 * @param activeRow The active row number
 * @param activeRowType The type of the active row This can be Normal_Commit, Branch_Commit, Merge_Branch_Commit
 * or a Merge_Commit
 * @param curMaxRows The size of the nextCommits vector
 * @param prevMaxRow The size of the previous commits nextCommits vector
 * @return
 */
QVector<enum Graph::GraphSymbol> *CommitGraph::buildUpLane(QSet<int> branchedToRow, QSet<int> mergeFromRow,
                                              QVector<Sha> &nextCommits, int activeRow,
                                              enum Graph::GraphSymbol activeRowType, int curMaxRows, int prevMaxRow)
{
    bool handledLastMerge = false;
    bool handledLastBranch = false;
    int handledMerges = 0;
    int handledBranches = 0;
    QVector<enum Graph::GraphSymbol> *currentRowState = new QVector<enum Graph::GraphSymbol>;

    for (int i = 0; i < curMaxRows; i++)
    {
        bool containsBranch = branchedToRow.contains(i);
        bool containsMerge = mergeFromRow.contains(i);
        Sha zero;
        Sha nextCommitSha = nextCommits[i];
        bool containsZero = &nextCommitSha == &zero;

        // anything before active row is an empty lane or non commit lane
        if (i < activeRow )
        {
            if (containsZero)
            {
                currentRowState->append(Graph::EMPTY_LANE);
            }
            else
            {
                currentRowState->append(Graph::NO_COMMIT);
            }
        }
        // we have worked out the lane type so no work needs to be done
        else if (i == activeRow)
        {
            currentRowState->append(activeRowType);
        }
        else if (activeRowType == Graph::BRANCH_COMMIT)
        {

            if (containsBranch)
            {
                handledLastBranch = appendRowTypeEqual (currentRowState, Graph::BRANCH_COMMIT_UP, handledBranches, branchedToRow.size());
            }
            else
            {
                handleRowTypeNotEqual (currentRowState, containsZero, handledLastBranch);
            }
        }
        else if (activeRowType == Graph::MERGE_COMMIT)
        {
            if (containsMerge)
            {
                handledLastMerge = appendRowTypeEqual (currentRowState, Graph::MERGE_COMMIT_DOWN, handledMerges, mergeFromRow.size() - 1);
            }
            else
            {
                handleRowTypeNotEqual (currentRowState, containsZero, handledLastMerge);
            }
        }
        else if (activeRowType == Graph::BRANCH_MERGE_COMMIT)
        {
            if (containsMerge && containsBranch)
            {
                handledMerges++;
                handledBranches++;

                if (handledMerges == mergeFromRow.size() -1)
                    handledLastMerge = true;

                if (handledBranches == branchedToRow.size())
                    handledLastBranch = true;

                if (handledLastBranch && handledLastMerge)
                    currentRowState->append(Graph::BRANCH_MERGE_COMMIT_BOTH);
                else
                    currentRowState->append(Graph::BRANCH_MERGE_COMMIT_BOTH_H);

            }
            else if (containsMerge)
            {
                handledLastMerge = branchMergeAppend (currentRowState, Graph::BRANCH_MERGE_COMMIT_DOWN, handledMerges,
                                   mergeFromRow.size() - 1, handledLastBranch);
            }
            else if (containsBranch)
            {
                handledLastBranch = branchMergeAppend (currentRowState, Graph::BRANCH_MERGE_COMMIT_UP, handledBranches,
                                   branchedToRow.size(), handledLastMerge);
            }
            else
            {
                bool handledBoth = handledLastBranch && handledLastMerge;
                handleRowTypeNotEqual (currentRowState, containsZero, handledBoth);

                if (!containsZero && i == curMaxRows -1 && i == branchedToRow.size() -1)
                {
                    currentRowState->append(Graph::BRANCH_MERGE_COMMIT_UP);
                }
            }
        }
        else if (activeRowType == Graph::NORMAL_COMMIT)
        {
            // add empty lanes or a single lane with no commit
            if (containsZero)
            {
                currentRowState->append(Graph::EMPTY_LANE);
            }
            else
            {
                currentRowState->append(Graph::NO_COMMIT);
            }
        }

    }

    branchsExtras (currentRowState, prevMaxRow, branchedToRow, activeRowType, handledBranches);

    return currentRowState;
}

/**
 * @brief acRepo::populateCommit This function works out what type of commit it is (eg a branch commit / a
 * merge commit / a branches merge commit or just a normal commit ). This information is then used
 * to create a Commit object.
 *
 * @param commit - The libgit2 commit object which on which we create the Commit out of
 * @param nextCommits - A Vector containing a list of the next oid to match against
 * @param prevCommit - The previous commit which is used to speed up creation of the lanes and
 * for the knowledge of the number of rows it contains.
 * @return A new Commit object.
 */
Graph* CommitGraph::populateCommit(Commit &commit, QVector<Sha> &nextCommits, Graph *prevCommit)
{
    enum Graph::GraphSymbol type = Graph::NO_COMMIT;
    bool mergeCommit = false;
    QSet<int> branchedToRow;
    QSet<int> mergeFromRow;
    int count = 0;
    int activeRow = -1;

    // count the next commits that have the same oid as the commit
    for (int nextCommitIndex = 0; nextCommitIndex < nextCommits.size(); nextCommitIndex++)
    {
        Sha nextCommit = nextCommits.at(nextCommitIndex);
        if (commit == &nextCommit)
        {
            // increament count of children with this commit
            if (count == 0)
            {
                activeRow = nextCommitIndex;
            }
            else
            {
                branchedToRow.insert(nextCommitIndex);
            }
            count++;
            //reset commit to zero
            Sha zero;
            nextCommits[nextCommitIndex] = zero;
        }
    }

    // reduces size of nextCommits
    for (int removeIndex = nextCommits.size() - 1; removeIndex > activeRow + 1; removeIndex--)
    {
        AcGit::Sha zero;
        if (nextCommits[removeIndex] == &zero)
        {
            nextCommits.remove(removeIndex);
        }
        else
        {
            // break early if index isn't a empty index.
            break;
        }
    }

    // Loops over parents of this commit and add to nextCommits List.
    for (int i = 0; i < commit.parentCount() && commit.parentCount() > 1; i++)
    {
        bool added = false;
        for (int k = activeRow; k < nextCommits.size(); k++)
        {
            //default value of git oid is 0
            Sha zero;
            if (nextCommits[k] == &zero)
            {
                nextCommits[k] = commit.parentSha(i);
                added = true;
                mergeFromRow.insert(k);
                break;
            }
        }
        if (added == false)
        {
            nextCommits.append(commit.parentSha(i));
            mergeFromRow.insert(nextCommits.size() - 1);
        }

        // more than one parent commit so must be a merge of some sort.
        mergeCommit = true;
    }

    if (mergeCommit && count == 1)
    {
        type = Graph::MERGE_COMMIT;
    }

    // if merge commit and child count > 1 then we have a merge commit
    if (mergeCommit && count > 1)
    {
        type = Graph::BRANCH_MERGE_COMMIT;
    }

    // child count is more than 1 so we have a branch
    if (!mergeCommit && count > 1)
    {
        // remove current commits that match commit
        type = Graph::BRANCH_COMMIT;

        // make sure this isn't the last commit
        if (commit.parentCount() != 0)
        {
            for (int k = activeRow; k < nextCommits.size(); k++)
            {
                Sha zero;
                if (nextCommits[k] == &zero)
                {
                    if (k < nextCommits.size())
                    {
                        nextCommits[k] = commit.parentSha(0);
                    }
                    else
                    {
                        nextCommits.insert(k, commit.parentSha(0));
                    }
                    break;
                }
            }
        }
    }

    // only one parent and only one child means this must be a commits
    if (!mergeCommit && count == 1)
    {
        type = Graph::NORMAL_COMMIT;
        if (commit.parentCount() != 0)
        {
             nextCommits[activeRow] = commit.parentSha(0);
        }
    }

    // an error has occured dump info so it can be debugged
    if (count == 0)
    {
        qDebug() << "commit oid = " << commit.toString();
        int a = 0;
        foreach (Sha oid, nextCommits)
        {
            qDebug() << "next commit in lane " << a << " oid = " << oid.toString();
            a++;
        }

        qDebug() << "I found an orphan node";
        return nullptr;
    }

    QVector<enum Graph::GraphSymbol> *laneState;
    if(type == Graph::NORMAL_COMMIT && prevCommit && prevCommit->activeRowNumber() == activeRow )
    {
        laneState = prevCommit->rowState();
    }
    else
    {
        int maxRows;
        if (!prevCommit)
        {
            maxRows = 1;
        }
        else
        {
            maxRows = prevCommit->getTotalRows();
        }
        laneState = buildUpLane(branchedToRow, mergeFromRow, nextCommits, activeRow,type, nextCommits.size(), maxRows);
    }
    return new Graph (activeRow, laneState, nextCommits.size(), type);

}

}
