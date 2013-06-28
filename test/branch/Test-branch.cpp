
#include <assert.h>
#include <QString>
#include <QDebug>
#include <gtest/gtest.h>

#include "Repository.h"
#include "IBranches.h"
#include "Branch.h"
#include "../tap.h"

class BranchTest : public testing::Test 
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../testrepo/ ../tmptestrepo/");
        QString temp = "../tmptestrepo/";
        repo = new AcGit::Repository(temp);

        branchAgent = repo->BranchAgent();
    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::IBranches *branchAgent;
};


TEST_F(BranchTest, getAllBranches) {
    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();

    EXPECT_EQ(allBranches.count(), 3);
}

TEST_F(BranchTest, getAllBranchesTestRefname) {
    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();

    ASSERT_TRUE(allBranches.at(0)->getRefName().contains("refs/heads/master"));
    ASSERT_TRUE(allBranches.at(1)->getRefName().contains("refs/heads/temp"));
    ASSERT_TRUE(allBranches.at(2)->getRefName().contains("refs/heads/test"));
}

TEST_F(BranchTest, getAllBranchesTestBranchName) {
    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();

    ASSERT_TRUE(allBranches.at(0)->getBranchName().contains("master"));
    ASSERT_TRUE(allBranches.at(1)->getBranchName().contains("temp"));
    ASSERT_TRUE(allBranches.at(2)->getBranchName().contains("test"));
}

TEST_F(BranchTest, getAllBranchesTestOid) {
    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();

    ASSERT_TRUE(allBranches.at(0)->toString().contains("66c90b5404402bae7fe8d0535b206d21dd007b46"));
    ASSERT_TRUE(allBranches.at(1)->toString().contains("2146d67ec94c833be6b3e964e19125b2f9a81102"));
    ASSERT_TRUE(allBranches.at(2)->toString().contains("2146d67ec94c833be6b3e964e19125b2f9a81102"));
}

TEST_F(BranchTest, getLocalBranches) {
    QList<AcGit::Branch*> localBranches = branchAgent->getLocalBranches();

    EXPECT_EQ(localBranches.count(), 3);
}

TEST_F(BranchTest, getLocalBranchesTestRefname) {
    QList<AcGit::Branch*> localBranches = branchAgent->getLocalBranches();

    qDebug() << localBranches.at(0)->getRefName();
    qDebug() << localBranches.at(1)->getRefName();
    qDebug() << localBranches.at(2)->getRefName();
    ASSERT_TRUE(localBranches.at(0)->getRefName().contains("refs/heads/master"));
    ASSERT_TRUE(localBranches.at(1)->getRefName().contains("refs/heads/temp"));
    ASSERT_TRUE(localBranches.at(2)->getRefName().contains("refs/heads/test"));
}

TEST_F(BranchTest, getLocalBranchesTestBranchName) {
    QList<AcGit::Branch*> localBranches = branchAgent->getLocalBranches();

    ASSERT_TRUE(localBranches.at(0)->getBranchName().contains("master"));
    ASSERT_TRUE(localBranches.at(1)->getBranchName().contains("temp"));
    ASSERT_TRUE(localBranches.at(2)->getBranchName().contains("test"));
}

TEST_F(BranchTest, getLocalBranchesTestOid) {
    QList<AcGit::Branch*> localBranches = branchAgent->getLocalBranches();

    ASSERT_TRUE(localBranches.at(0)->toString().contains("66c90b5404402bae7fe8d0535b206d21dd007b46"));
    ASSERT_TRUE(localBranches.at(1)->toString().contains("2146d67ec94c833be6b3e964e19125b2f9a81102"));
    ASSERT_TRUE(localBranches.at(2)->toString().contains("2146d67ec94c833be6b3e964e19125b2f9a81102"));
}


TEST_F(BranchTest, branchNameTest) {
    QStringList branchNames = branchAgent->branchNamesList();

    EXPECT_EQ(branchNames.count(), 3);

    ASSERT_TRUE(branchNames.at(0).contains("master"));
    ASSERT_TRUE(branchNames.at(1).contains("temp"));
    ASSERT_TRUE(branchNames.at(2).contains("test"));
}

TEST_F(BranchTest, branchLookupSha) {
    QList<AcGit::Branch *> branchesFound = branchAgent->lookupBranch(repo->HeadCommit());

    EXPECT_EQ(branchesFound.count(), 1);

    ASSERT_TRUE(branchesFound.at(0)->getBranchName().contains("master"));
    ASSERT_TRUE(branchesFound.at(0)->toString().contains("66c90b5404402bae7fe8d0535b206d21dd007b46"));
}


TEST_F(BranchTest, createBranch) {

    AcGit::Commit *commit = repo->HeadCommit();
    ASSERT_TRUE (commit != nullptr);

    branchAgent->createNewBranch("newBranch",commit,1);

    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();
    EXPECT_EQ (allBranches.count(), 4);

    AcGit::Branch *newBranch = branchAgent->lookupBranch("refs/heads/newBranch");
    ASSERT_TRUE (newBranch != nullptr);

}

TEST_F(BranchTest, deleteBranchByName) {
    AcGit::Branch *deleteBranch = branchAgent->lookupBranch("refs/heads/test");
    branchAgent->deleteBranch(deleteBranch);

    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();
    EXPECT_EQ (allBranches.count(), 2);

    AcGit::Branch *deletedBranch = branchAgent->lookupBranch("refs/heads/test");
    ASSERT_TRUE(deletedBranch == nullptr);
}

TEST_F(BranchTest, moveBranchToName) {
    AcGit::Branch *branch = branchAgent->lookupBranch("refs/heads/test");
    branchAgent->moveBranch(branch, "newTest");

    QList<AcGit::Branch*> allBranches = branchAgent->getAllBranches();
    EXPECT_EQ (allBranches.count(), 3);

    AcGit::Branch *newNameBranch = branchAgent->lookupBranch ("refs/heads/newTest");
    ASSERT_TRUE(newNameBranch != nullptr);
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
