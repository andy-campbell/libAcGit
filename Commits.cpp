#include <QDebug>

#include "Commits.h"

namespace AcGit
{

Commits::Commits(Repository *repo, SortMode mode)
{
    this->repo = repo;
    this->currentSortMode = mode;
    CommitsList =  new QList<Commit*>();

    git_revwalk *walker;
    try
    {

        gitTest(git_revwalk_new(&walker, repo->getInternalRepo()));

        applySortMode(walker);

        setStartCommit(walker);

        startRevWalk (walker);

    }
    catch (GitException e)
    {
        revNewWalkerError (e);
    }

}


void Commits::gitRevWalkNextException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}

void Commits::startRevWalk(git_revwalk* walker)
{
    git_oid sha;
    try
    {
        while (gitTest(git_revwalk_next(&sha, walker) != GIT_ITEROVER))
        {
            git_oid *shaCopy = (git_oid *)malloc (sizeof(struct git_oid));
            git_oid_cpy(shaCopy, &sha);

            Sha *shaForCommit = new Sha(shaCopy);
            Commit* commit = new Commit(repo, shaForCommit);
            CommitsList->append(commit);
            lookupMap.insert(shaForCommit->toString(), commit);
        }
    }
    catch (GitException e)
    {
        gitRevWalkNextException(e);
    }


}

void Commits::gitRevWalkPushHeadException (GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}


void Commits::setStartCommit(git_revwalk* walker)
{
    try
    {
        git_revwalk_push_head(walker);
    }
    catch (GitException e)
    {
        gitRevWalkPushHeadException (e);
    }
}

void Commits::revNewWalkerError(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}

void Commits::applySortMode(git_revwalk *walker)
{
    if (currentSortMode == SORT_NONE)
    {
        git_revwalk_sorting(walker, currentSortMode);
    }
    else
    {
        git_revwalk_sorting(walker, 1 << currentSortMode);
    }

}

QList<Commit *> *Commits::getAllCommits()
{
    return CommitsList;
}

void Commits::setSortingMode(SortMode mode)
{
    this->currentSortMode = mode;
}

Commit *Commits::lookupCommit(Sha *sha)
{
    return lookupMap.find(sha->toString()).value();
}

}
