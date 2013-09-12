/*

Copyright (c) 2013 Andrew Campbell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <QDebug>

#include "Commits.h"
#include "Commitgraph.h"

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

        applySortMode(walker, false);

        setStartCommit(walker, nullptr);

        startRevWalk (walker, false);

        generateGraph();

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

void Commits::startRevWalk(git_revwalk* walker, bool update)
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

            if(update)
            {
                CommitsList->insert(CommitsList->begin(), commit);
            }
            else
            {
                CommitsList->append(commit);
            }
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


void Commits::setStartCommit(git_revwalk* walker, Sha *shaToWalkFrom)
{
    try
    {
        if(shaToWalkFrom)
        {
            gitTest(git_revwalk_push(walker, shaToWalkFrom->raw()));
        }
        else
        {
            gitTest(git_revwalk_push_head(walker));
        }
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

void Commits::applySortMode(git_revwalk *walker, bool reverse)
{
    if (currentSortMode == SORT_NONE)
    {
        git_revwalk_sorting(walker, currentSortMode);
    }
    else
    {
        int mode = 1 << currentSortMode;
        if(reverse)
            mode = mode | 1 << SORT_REVERSE;

        git_revwalk_sorting(walker, mode);
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
    Commit *commit = nullptr;
    QMap<QString ,Commit *>::iterator commitItor;
    commitItor = lookupMap.find(sha->toString());
    if (commitItor != lookupMap.end())
    {
        commit = commitItor.value();
    }

    return commit;
}

void Commits::updateCommits(int difference)
{
    if(difference < 0)
    {
        for(int index = 0; index < -difference; index++)
        {
            Commit* commit = CommitsList->at(0);
            lookupMap.remove(commit->toString());
            CommitsList->removeFirst();
            delete commit;
        }
    }

    if(difference > 0)
    {
        Commit *commitFrom = CommitsList->at(0);
        git_revwalk *walker;
        gitTest(git_revwalk_new(&walker, repo->getInternalRepo()));

        applySortMode(walker, true);

        setStartCommit(walker, commitFrom);

        startRevWalk(walker, true);
    }
}

void Commits::generateGraph()
{
    CommitGraph commitGraph(CommitsList);
    Q_UNUSED(commitGraph);
}

}
