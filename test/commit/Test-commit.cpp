
#include <assert.h>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <gtest/gtest.h>

#include "Repository.h"
#include "IBranches.h"
#include "Branch.h"
#include "../tap.h"

static int numTests = 12;

class CommitTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../testrepo/ ../tmptestrepo/");
        QString temp = "/home/andrew/build/acgit/libAcGit/test/tmptestrepo";
        repo = new AcGit::Repository(temp);
        commitsAgent = repo->CommitsAgent();
    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::ICommits *commitsAgent;
};


TEST_F(CommitTest, test_for_commitsAgent) {
    ASSERT_TRUE (commitsAgent != nullptr);
}

TEST_F(CommitTest, test_for_commitsAgentCount) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    // test commit count
    EXPECT_EQ(listOfCommits->count(), 2);
}

TEST_F(CommitTest, test_for_getCommitcommitsAgent) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();


    ASSERT_TRUE (listOfCommits->first() != nullptr);
}

TEST_F(CommitTest, test_for_testAuthor) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit != nullptr);
    QString author = commit->author();

    EXPECT_EQ(author.contains("Andrew campbell"), true);
}

TEST_F(CommitTest, test_for_authorsEmail) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QString authorEmail = commit->authorsEmail();

    qDebug() << commit->authorsEmail();
    EXPECT_EQ(authorEmail.contains("andrew.campbell@xtra.co.nz"), true);
}

TEST_F(CommitTest, test_for_commitTree) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit != nullptr);

    ASSERT_TRUE (commit->tree() != nullptr);
}


TEST_F(CommitTest, test_for_dateCreated) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QDateTime dateCreated = commit->dateCreated();

    EXPECT_EQ(dateCreated.toString().contains("Thu May 9 21:05:43 2013"), true);
}

TEST_F(CommitTest, test_for_commiter) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QString commiter = commit->committer();

    EXPECT_EQ(commiter.contains("Andrew campbell"), true);
}

TEST_F(CommitTest, test_for_commiterEmail) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QString commiterEmail = commit->committerEmail();

    EXPECT_EQ(commiterEmail.contains("andrew.campbell@xtra.co.nz"), true);
}

TEST_F(CommitTest, test_for_commitLog) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QString log = commit->log();
    QString expectedLog = "Added to test repo\n\nAdded another commit to the test repo\n";

    EXPECT_EQ(log.contains(expectedLog), true);
}

TEST_F(CommitTest, test_for_commitShortLog) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QString shortLog = commit->shortLog();
    QString expectedShortLog = "Added to test repo";

    EXPECT_EQ(shortLog.contains(expectedShortLog), true);
}

TEST_F(CommitTest, test_for_commitOid) {
    QList<AcGit::Commit *> *listOfCommits = commitsAgent->getAllCommits();

    AcGit::Commit *commit = listOfCommits->first();
    ASSERT_TRUE (commit);
    QString oidString = commit->toString();
    QString expectedOidString = "66c90b5404402bae7fe8d0535b206d21dd007b46";
    qDebug() << commit->toString();
    //TODO verify
    EXPECT_EQ(oidString.contains(expectedOidString), true);
}

int main (int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
    qDebug() << "This test set contains " << numTests << " test";
    // Delete the default listener
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new tap::TapListener());

    return RUN_ALL_TESTS();
}
