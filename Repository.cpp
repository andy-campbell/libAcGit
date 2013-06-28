#include "Repository.h"
#include "Branches.h"
#include "Commits.h"
#include "Gitexception.h"
#include "Tag.h"
#include "Tags.h"

#include <QDebug>
#include <QList>

namespace AcGit
{


Repository::Repository(QString path)
{
    try
    {
        gitTest (git_repository_open(&repo, path.toLocal8Bit()));

        branches = new Branches(repo);
        commits = new Commits(this);
        tags = new Tags(this);

    }
    catch (GitException e)
    {
        qDebug() << e.exceptionMessage();
    }
}

Repository::~Repository()
{
    delete branches;
    git_repository_free (repo);
}

IBranches *Repository::BranchAgent()
{
    return branches;
}

ICommits *Repository::CommitsAgent()
{
    return commits;
}

ITags *Repository::TagsAgent()
{
    return tags;
}

git_repository* Repository::getInternalRepo()
{
    return repo;
}

Commit* Repository::HeadCommit()
{
    Commit *commit = nullptr;
    try
    {
        git_reference *headRef;
        gitTest(git_repository_head(&headRef, repo));

        const git_oid *sha = git_reference_target(headRef);
        Sha searchSha(sha);
        commit = commits->lookupCommit(&searchSha);

    }
    catch(GitException e)
    {
        qDebug() << e.exceptionMessage();
    }
    return commit;
}


}
