
#include <assert.h>
#include <QString>
#include <QDebug>
#include <QList>

#include "Repository.h"
#include "ICommits.h"
#include "Commit.h"

int main (int argc, char **argv)
{
    QString temp = "../testrepo/";
    AcGit::Repository *repo = new AcGit::Repository(temp);

    assert (repo != nullptr);

    QList<AcGit::Commit*> *commits = repo->getCommits();

    if (commits->isEmpty())
    {
        qCritical() << "wrong number of commits found";
        assert (commits->size() == 0);
    }

    foreach (AcGit::Commit* commit, *commits)
    {
        qDebug() << "ShortLog: " << commit->shortLog();
        qDebug() << "Author: " <<commit->author();
        qDebug() << "Authors Email: " << commit->authorsEmail();
        qDebug() << "Commiter: " << commit->committer();
        qDebug() << "Commiter Email: " << commit->committerEmail();
        qDebug() << "Created Date: " << commit->dateCreated().toString();
        qDebug() << "Log: " << commit->log();

    }
}
