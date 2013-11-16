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

#include "Repository.h"
#include "Branches.h"
#include "Commits.h"
#include "Gitexception.h"
#include "Tag.h"
#include "Tags.h"
#include "Reset.h"
#include "Workingdirdiff.h"
#include "Stagingdirdiff.h"

#include <QDebug>
#include <QList>

namespace AcGit
{


Repository::Repository(QString path)
{
    char pathOut[512];
    gitTest(git_repository_discover(pathOut, 512, path.toLocal8Bit(), true, "/"));
    gitTest(git_repository_open(&repo, pathOut));
    initaliseRepo();
}

Repository::Repository(git_repository *internalRepo)
{
    repo = internalRepo;
    initaliseRepo();
}

void Repository::initaliseRepo()
{
    Sha *headSha = getRepositoryHead();
    qDebug() << headSha->toString();
    branches = new Branches(repo);
    branchHead = dynamic_cast<Branches*>(branches)->getActiveBranch();
    commits = new Commits(this, headSha);
    tags = new Tags(this);
    config = new Configuration(this);
    reset = new Reset(this);

}

Repository::~Repository()
{
    delete branches;
    delete commits;
    delete tags;
    delete config;
    delete reset;

    git_repository_free (repo);
}

/* Caller must free Sha */
Sha *Repository::getRepositoryHead()
{
    Sha* headSha = nullptr;
    try
    {
        git_reference *headRef;
        gitTest(git_repository_head(&headRef, repo));

        const git_oid *sha = git_reference_target(headRef);
        headSha = new Sha(sha);
    }
    catch(GitException e)
    {
        qDebug() << e.exceptionMessage();
    }
    return headSha;
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

IReset *Repository::ResetAgent()
{
    return reset;
}

git_repository* Repository::getInternalRepo()
{
    return repo;
}

Configuration* Repository::ConfigurationAgent()
{
    return config;
}

Commit* Repository::HeadCommit()
{
    Commit *commit = nullptr;
    Sha *headSha = getRepositoryHead();
    if (headSha)
    {
        commit = commits->lookupCommit(headSha);

    }
    return commit;
}

Commit* Repository::ActiveBranchCommit()
{
    Commit *commit = nullptr;
    try
    {
        commit = commits->lookupCommit(branchHead);

    }
    catch(GitException e)
    {
        qDebug() << e.exceptionMessage();
    }
    return commit;
}


bool Repository::hasChanges(Diff &diff)
{
    bool hasChanges = false;
    if (diff.fileList().size() != 0)
    {
        hasChanges = true;
    }

    return hasChanges;
}

bool Repository::HasWorkingTreeChanges()
{
    Sha* headSha = getRepositoryHead();
    Commit* headCommit = commits->lookupCommit(headSha);
    WorkingDirDiff diff(headCommit->tree());
    return hasChanges(diff);
}

bool Repository::HasStagingDirChanges()
{
    Sha* headSha = getRepositoryHead();
    Commit* headCommit = commits->lookupCommit(headSha);
    StagingDirDiff diff(headCommit->tree());
    return hasChanges(diff);
}

QString Repository::GitTopLevelDirectory()
{
    QString returnedPath = "";
    const char *path = git_repository_path(repo);
    if (path != nullptr)
        returnedPath = QString (path);

    return returnedPath;
}

int Repository::AheadBehind(Sha* from, Sha* to)
{
    size_t ahead;
    size_t behind;

    gitTest(git_graph_ahead_behind(&ahead, &behind, repo, from->raw(), to->raw()));

    return ahead - behind;
}


//TODO make this function more efficient. Don't reload everything.
void Repository::Update()
{
    Sha *oldBranchSha = new Sha(branchHead->raw());
    dynamic_cast<Branches*>(branches)->updateActiveBranch();
    Sha *newBranchHead = dynamic_cast<Branches*>(branches)->getActiveBranch();

    int numCommitDifference = AheadBehind(oldBranchSha, newBranchHead);
    if(numCommitDifference == 0)
        return;

    branchHead = newBranchHead;

    dynamic_cast<Commits*>(commits)->updateCommits(newBranchHead);
    dynamic_cast<Tags*>(tags)->upateTags();
    delete oldBranchSha;
}

}
