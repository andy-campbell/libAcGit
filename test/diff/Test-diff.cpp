
#include <assert.h>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QList>

#include "Repository.h"
#include "ICommits.h"
#include "Commit.h"
#include "TreeDiff.h"

#include <git2.h>


#include <assert.h>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <gtest/gtest.h>

#include "Repository.h"
#include "IBranches.h"
#include "Branch.h"
#include "../tap.h"

static int numTests = 7;

class DiffTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../testrepo/ ../tmptestrepo/");
        QString temp = "../tmptestrepo/";
        repo = new AcGit::Repository(temp);

    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::ICommits *commitsAgent;
};


TEST_F(DiffTest, test_for_treeDiffIsnotNull) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(0);
    AcGit::Commit *commitB = commits->getAllCommits()->at(1);

    ASSERT_TRUE(commitA != nullptr);
    ASSERT_TRUE(commitB != nullptr);

    AcGit::Diff *diff = new AcGit::TreeDiff(commitA->tree(), commitB->tree());

    ASSERT_TRUE (diff != nullptr);

}

TEST_F(DiffTest, test_for_treeDiffWrongCommitOrderl) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(0);
    AcGit::Commit *commitB = commits->getAllCommits()->at(1);

    ASSERT_TRUE(commitA != nullptr);
    ASSERT_TRUE(commitB != nullptr);

    AcGit::Diff *diff = new AcGit::TreeDiff(commitB->tree(), commitA->tree());

    ASSERT_TRUE (diff != NULL);
}


TEST_F(DiffTest, test_for_CommitBisNull) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(0);

    ASSERT_TRUE(commitA != nullptr);

    AcGit::Diff *diff;
    EXPECT_THROW(diff = new AcGit::TreeDiff(commitA->tree(), nullptr), AcGit::ApiException);

}

TEST_F(DiffTest, test_for_toCommitisNull) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(1);

    ASSERT_TRUE(commitA != nullptr);

    //tests for initial commit to git repo
    AcGit::Diff *diff = new AcGit::TreeDiff(nullptr, commitA->tree());
    ASSERT_TRUE (diff != NULL);
}


TEST_F(DiffTest, test_for_diffStats) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(0);
    AcGit::Commit *commitB = commits->getAllCommits()->at(1);

    ASSERT_TRUE(commitA != nullptr);
    ASSERT_TRUE(commitB != nullptr);

    AcGit::Diff *diff = new AcGit::TreeDiff(commitA->tree(), commitB->tree());

    ASSERT_TRUE (diff != nullptr);
    qDebug() << diff->diffStats();
    QString expectedStats = QString(
    " abcd\t|    -\n 1 file changed, 0 insertions(+), 1 deletions(-)\n");
    qDebug() << "stats";
    for ( int i = 0; i < diff->diffStats().length(); i++)
    {
        qDebug("%d ", diff->diffStats().at(i).toLatin1());
    }
    qDebug() << "expected";
    for ( int i = 0; i < expectedStats.length(); i++)
    {
        qDebug("%d ", expectedStats.at(i).toLatin1());
    }


    EXPECT_EQ(diff->diffStats().contains(expectedStats), true);
}

TEST_F(DiffTest, test_for_diff) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(0);
    AcGit::Commit *commitB = commits->getAllCommits()->at(1);

    ASSERT_TRUE(commitA != nullptr);
    ASSERT_TRUE(commitB != nullptr);

    AcGit::Diff *diff = new AcGit::TreeDiff(commitA->tree(), commitB->tree());
    

    ASSERT_TRUE (diff != nullptr);

    QString expectedDiff = QString(
    "diff --git a/abcd b/abcd\n"
    "index f4001ea..e69de29 100644\n"
    "--- a/abcd\n"
    "+++ b/abcd\n"
    "@@ -1 +0,0 @@\n"
    "-A Test Repo\n"
    ""
    );


    EXPECT_EQ(diff->printDiff().contains(expectedDiff), true);
}


TEST_F(DiffTest, test_for_fileList) {
    AcGit::ICommits *commits = repo->CommitsAgent();

    ASSERT_TRUE(commits != nullptr);

    AcGit::Commit *commitA = commits->getAllCommits()->at(0);
    AcGit::Commit *commitB = commits->getAllCommits()->at(1);

    ASSERT_TRUE(commitA != nullptr);
    ASSERT_TRUE(commitB != nullptr);

    AcGit::Diff *diff = new AcGit::TreeDiff(commitA->tree(), commitB->tree());

    ASSERT_TRUE (diff != nullptr);

    QStringList expectedFilesStringList;
    expectedFilesStringList << "abcd";
    QStringList diffFileStringList = diff->fileList();
    foreach (QString fileName, expectedFilesStringList)
    {
        diffFileStringList.removeOne(fileName);
    }
    qDebug() << diffFileStringList;
    EXPECT_EQ(diffFileStringList.count(), 0);


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



//int main (int argc, char **argv)
//{
//    qDebug() << "Diff";
//    QString temp = "../testrepo";
//    AcGit::Repository *repo = new AcGit::Repository(temp);

//    assert (repo != nullptr);

//    QList<AcGit::Commit*> *commits = repo->getCommits();

//    if (commits->isEmpty())
//    {
//        qCritical() << "wrong number of commits found";
//        assert (commits->size() == 0);
//    }

//    AcGit::Commit *firstCommit = commits->first();
//    AcGit::Commit *secondCommit = commits->last();

//    if (firstCommit == secondCommit)
//    {
//        qDebug() << "same commit found";
//        assert(0);
//    }

//    AcGit::TreeDiff *treediff1 = new AcGit::TreeDiff(secondCommit->tree(), firstCommit->tree());
//    AcGit::TreeDiff *treediff2 = new AcGit::TreeDiff(nullptr, secondCommit->tree());

//    foreach (QString fileChanged, treediff1->fileList())
//    {
//        qDebug() << "File Changed: " << fileChanged;
//        qDebug() << treediff1->fileDiff(fileChanged);
//    }

//    foreach (QString fileChanged, treediff2->fileList())
//    {
//        qDebug() << "File Changed: " << fileChanged;
//        qDebug() << treediff2->fileDiff(fileChanged);
//    }

//    qDebug() << "exit";

//}
