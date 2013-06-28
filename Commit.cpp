#include <QDebug>
#include <QStringList>
#include <QRegExp>

#include "Commit.h"

namespace AcGit
{

Commit::Commit(Repository *repo, Sha *commitSha)
    : Sha (commitSha)
{

    this->repo = repo;
    try
    {
        gitTest(git_commit_lookup(&commit, repo->getInternalRepo(), sha));
    }
    catch (GitException e)
    {
        commitLookupExecption (e);
    }
}

Commit::~Commit()
{

}

void Commit::commitLookupExecption(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}


QString Commit::log() const
{
    QString log = QString(git_commit_message(commit));
    return log;
}


QString Commit::shortLog()
{
    QRegExp regex("(\\r|\\n)");
    QString shortLog = log().left(80).split(regex).first();
    return shortLog;
}

QString Commit::author()
{
    const git_signature* signature = git_commit_author(commit);

    return QString(signature->name);
}

QString Commit::authorsEmail()
{
    const git_signature* signature = git_commit_author(commit);

    return QString(signature->email);
}

QString Commit::committer()
{
    const git_signature* signature = git_commit_committer (commit);

    return QString(signature->email);
}

QString Commit::committerEmail()
{
    const git_signature* signature = git_commit_committer (commit);

    return QString(signature->email);
}

Tree *Commit::tree() const
{
    git_tree *gitTree;
    gitTest(git_commit_tree (&gitTree, commit));

    Tree* commitTree = new Tree (repo, gitTree);
    return commitTree;
}

git_commit *Commit::rawCommit()
{
    return commit;
}


QDateTime Commit::dateCreated()
{
    QDateTime dateTime;
    git_time_t time = git_commit_time(commit);
    dateTime.setTime_t(time);
    return dateTime;
}

}
