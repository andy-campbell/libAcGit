
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

int main (int argc, char **argv)
{
    qDebug() << "Diff";
    QString temp = "../testrepo";
    AcGit::Repository *repo = new AcGit::Repository(temp);

    assert (repo != nullptr);

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

}
