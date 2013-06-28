
#include <assert.h>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <gtest/gtest.h>

#include "Repository.h"
#include "ITags.h"
#include "Tag.h"
#include "Tagger.h"
#include "../tap.h"



class TagTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        system("cp -r ../testrepo/ ../tmptestrepo/");
        QString temp = "../tmptestrepo/";
        repo = new AcGit::Repository(temp);

        tagsAgent = repo->TagsAgent();
    }

    virtual void TearDown() {
        system("rm -rf ../tmptestrepo/");
    }
    AcGit::Repository *repo;
    AcGit::ITags *tagsAgent;
};

TEST_F(TagTest, getTagsCount) {
    QList<AcGit::Tag *> tagsList = tagsAgent->listTags();

    EXPECT_EQ(tagsList.count(), 4);
}

TEST_F(TagTest, getTagsList) {
    QStringList tagsList = tagsAgent->stringTagsList();

    ASSERT_TRUE(tagsList.at(0).contains("aTag"));
    ASSERT_TRUE(tagsList.at(1).contains("anotherTag"));
    ASSERT_TRUE(tagsList.at(2).contains("first"));
    ASSERT_TRUE(tagsList.at(3).contains("messageTag"));

}

TEST_F(TagTest, lookupTag) {
    AcGit::Tag *tag = tagsAgent->lookupTag ("aTag");

    ASSERT_TRUE (tag != nullptr);

    ASSERT_TRUE (tag->name().contains("aTag"));
    ASSERT_TRUE (!tag->name().contains("/aTag"));
    ASSERT_TRUE (tag->toString().contains("66c90b5404402bae7fe8d0535b206d21dd007b46"));
}

TEST_F(TagTest, createTag) {
    AcGit::Commit *commit = repo->HeadCommit();
    AcGit::Tagger *tagger = new AcGit::Tagger("Andrew", "Hello@test.com");
    tagsAgent->createTag ("newTag", commit, tagger, "hurray we created a tag");

    QList<AcGit::Tag *> tagsList = tagsAgent->listTags();
    EXPECT_EQ(tagsList.count(), 5);

    AcGit::Tag *tag = tagsAgent->lookupTag("newTag");
    ASSERT_TRUE(tag != nullptr);

    AcGit::Tagger *newTagger = tag->getTagger();
    ASSERT_TRUE(newTagger->email().contains("Hello@test.com"));
    ASSERT_TRUE(newTagger->name().contains("Andrew"));
}

TEST_F(TagTest, deleteTag) {
    AcGit::Tag *tag = tagsAgent->lookupTag("first");
    ASSERT_TRUE(tag != nullptr);
    qDebug() << tag->name();
    tagsAgent->deleteTag(tag);

    QList<AcGit::Tag *> tagsList = tagsAgent->listTags();
    EXPECT_EQ(tagsList.count(), 3);

    AcGit::Tag *newTag = tagsAgent->lookupTag("newTag");
    ASSERT_TRUE(newTag == nullptr);
}

TEST_F(TagTest, tagMessage) {
    AcGit::Tag *tag = tagsAgent->lookupTag("messageTag");

    QString message = tag->message();
    ASSERT_TRUE(message.contains("this is a message"));
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
