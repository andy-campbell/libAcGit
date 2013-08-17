
#include <assert.h>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QVector>

#include "Repository.h"
#include "ICommits.h"
#include "Commit.h"
#include "TreeDiff.h"
#include "Graph.h"


#include <git2.h>


#include <assert.h>
#include <QString>
#include <QDateTime>
#include <gtest/gtest.h>

#include "Repository.h"
#include "IBranches.h"
#include "Branch.h"
#include "../tap.h"

class GraphTestBasic : public testing::Test
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../testrepo/ ../tmptestrepo/");
        QString temp = "../tmptestrepo/";
        repo = new AcGit::Repository(temp);
        commitsAgent = repo->CommitsAgent();

    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::ICommits *commitsAgent;
};


TEST_F(GraphTestBasic, test_for_basicRepoActiveRow) {
    AcGit::Commit *firstCommit = commitsAgent->getAllCommits()->first();

    AcGit::Graph *graphOfFirstCommit = firstCommit->getGraph();

    EXPECT_EQ (graphOfFirstCommit->activeRowNumber(), 0);
}

TEST_F(GraphTestBasic, test_for_basicRepoCommitType) {

    AcGit::Commit *firstCommit = commitsAgent->getAllCommits()->first();

    AcGit::Graph *graphOfFirstCommit = firstCommit->getGraph();

    EXPECT_EQ (graphOfFirstCommit->typeOfRow(), AcGit::Graph::NORMAL_COMMIT);
}

TEST_F(GraphTestBasic, test_for_basicRepoTotalRows) {
    AcGit::Commit *firstCommit = commitsAgent->getAllCommits()->first();

    AcGit::Graph *graphOfFirstCommit = firstCommit->getGraph();

    EXPECT_EQ (graphOfFirstCommit->getTotalRows(), 1);
}

TEST_F(GraphTestBasic, test_for_basicRepoRowState) {
    AcGit::Commit *firstCommit = commitsAgent->getAllCommits()->first();

    AcGit::Graph *graphOfFirstCommit = firstCommit->getGraph();

    EXPECT_EQ (graphOfFirstCommit->rowState()->at(0), AcGit::Graph::NORMAL_COMMIT);
}


class GraphTestMergeBranch : public testing::Test
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../branchmergerepo/ ../tmptestrepo/");
        QString temp = "../tmptestrepo/";
        repo = new AcGit::Repository(temp);
        commitsAgent = repo->CommitsAgent();

    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::ICommits *commitsAgent;
};

/**
 * Commit order form their shorts
 *
 * Merge Commit
 * Commit Work Branch
 * Commit Master xyz
 * Test Commit
 *
 */

TEST_F(GraphTestMergeBranch, test_for_ActiveRowCommit) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit= commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 0);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 0);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 1);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 0);
}

TEST_F(GraphTestMergeBranch, test_for_repoCommitType) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;
    int expected;
    int got;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::MERGE_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::NORMAL_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::NORMAL_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::BRANCH_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);
}

TEST_F(GraphTestMergeBranch, test_for_RepoTotalRows) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 2);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 2);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 2);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 2);

}

TEST_F(GraphTestMergeBranch, test_for_RepoRowState) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;
    QVector<enum AcGit::Graph::GraphSymbol> *laneState;
    int got0,got1;
    int expected0, expected1;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit= commit->getGraph();
    laneState = graphOfCommit->rowState();
    got0 = laneState->at(0); expected0 = AcGit::Graph::MERGE_COMMIT;
    got1 = laneState->at(1); expected1 = AcGit::Graph::MERGE_COMMIT_DOWN;
    EXPECT_EQ (got0, expected0);
    EXPECT_EQ (got1, expected1);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit= commit->getGraph();
    laneState = graphOfCommit->rowState();
    got0 = laneState->at(0); expected0 = AcGit::Graph::NORMAL_COMMIT;
    got1 = laneState->at(1); expected1 = AcGit::Graph::NO_COMMIT;
    EXPECT_EQ (got0, expected0);
    EXPECT_EQ (got1, expected1);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit= commit->getGraph();
    laneState = graphOfCommit->rowState();
    got0 = laneState->at(0); expected0 = AcGit::Graph::NO_COMMIT;
    got1 = laneState->at(1); expected1 = AcGit::Graph::NORMAL_COMMIT;
    EXPECT_EQ (got0, expected0);
    EXPECT_EQ (got1, expected1);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit= commit->getGraph();
    laneState = graphOfCommit->rowState();
    got0 = laneState->at(0); expected0 = AcGit::Graph::BRANCH_COMMIT;
    got1 = laneState->at(1); expected1 = AcGit::Graph::BRANCH_COMMIT_UP;
    EXPECT_EQ (got0, expected0);
    EXPECT_EQ (got1, expected1);
}



class GraphTestMultiMergeBranch : public testing::Test
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../multimergerepo/ ../tmptestrepo/");
        QString temp = "../tmptestrepo/";
        repo = new AcGit::Repository(temp);
        commitsAgent = repo->CommitsAgent();

    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::ICommits *commitsAgent;
};


/**
 * Commit order form their shorts
 *
 * Merge Commit
 * Commit Work Branch
 * Commit Master xyz
 * Test Commit
 *
 */

TEST_F(GraphTestMultiMergeBranch, test_for_ActiveRowCommit) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;



    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit= commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 0);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 0);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 2);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 1);

    // commit index 4
    commit = commitsAgent->getAllCommits()->at(4);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->activeRowNumber(), 0);

}

TEST_F(GraphTestMultiMergeBranch, test_for_repoCommitType) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;
    int got;
    int expected;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::MERGE_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::NORMAL_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::NORMAL_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::NORMAL_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

    // commit index 4
    commit = commitsAgent->getAllCommits()->at(4);
    graphOfCommit= commit->getGraph();
    expected = AcGit::Graph::BRANCH_COMMIT;
    got = graphOfCommit->typeOfRow();
    EXPECT_EQ (got, expected);

}

TEST_F(GraphTestMultiMergeBranch, test_for_RepoTotalRows) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 3);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 3);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 3);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 3);

    // commit index 4
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit = commit->getGraph();
    EXPECT_EQ (graphOfCommit->getTotalRows(), 3);

}

TEST_F(GraphTestMultiMergeBranch, test_for_RepoRowState) {
    AcGit::Commit *commit;
    AcGit::Graph *graphOfCommit;
    QVector<enum AcGit::Graph::GraphSymbol> *laneState;

    // commit index 0
    commit = commitsAgent->getAllCommits()->at(0);
    graphOfCommit= commit->getGraph();
    laneState = graphOfCommit->rowState();
    EXPECT_EQ (laneState->at(0), AcGit::Graph::MERGE_COMMIT);
    EXPECT_EQ (laneState->at(1), AcGit::Graph::MERGE_COMMIT_DOWN_H);
    EXPECT_EQ (laneState->at(2), AcGit::Graph::MERGE_COMMIT_DOWN);

    // commit index 1
    commit = commitsAgent->getAllCommits()->at(1);
    graphOfCommit = commit->getGraph();
    laneState = graphOfCommit->rowState();
    EXPECT_EQ (laneState->at(0), AcGit::Graph::NORMAL_COMMIT);
    EXPECT_EQ (laneState->at(1), AcGit::Graph::NO_COMMIT);
    EXPECT_EQ (laneState->at(2), AcGit::Graph::NO_COMMIT);

    // commit index 2
    commit = commitsAgent->getAllCommits()->at(2);
    graphOfCommit = commit->getGraph();
    laneState = graphOfCommit->rowState();
    EXPECT_EQ (laneState->at(0), AcGit::Graph::NO_COMMIT);
    EXPECT_EQ (laneState->at(1), AcGit::Graph::NO_COMMIT);
    EXPECT_EQ (laneState->at(2), AcGit::Graph::NORMAL_COMMIT);

    // commit index 3
    commit = commitsAgent->getAllCommits()->at(3);
    graphOfCommit = commit->getGraph();
    laneState = graphOfCommit->rowState();
    EXPECT_EQ (laneState->at(0), AcGit::Graph::NO_COMMIT);
    EXPECT_EQ (laneState->at(1), AcGit::Graph::NORMAL_COMMIT);
    EXPECT_EQ (laneState->at(2), AcGit::Graph::NO_COMMIT);

    // commit index 4
    commit = commitsAgent->getAllCommits()->at(4);
    graphOfCommit = commit->getGraph();
    laneState = graphOfCommit->rowState();
    EXPECT_EQ (laneState->at(0), AcGit::Graph::BRANCH_COMMIT);
    EXPECT_EQ (laneState->at(1), AcGit::Graph::BRANCH_COMMIT_UP_H);
    EXPECT_EQ (laneState->at(2), AcGit::Graph::BRANCH_COMMIT_UP);

}


int main (int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
    // Delete the default listener
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new tap::TapListener());

    return RUN_ALL_TESTS();
}


