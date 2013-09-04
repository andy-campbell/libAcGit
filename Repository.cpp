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
    branches = new Branches(repo);
    commits = new Commits(this);
    tags = new Tags(this);
    config = new Configuration(this);
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

Configuration* Repository::ConfigurationAgent()
{
    return config;
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
    WorkingDirDiff diff(HeadCommit()->tree());
    return hasChanges(diff);
}

bool Repository::HasStagingDirChanges()
{
    StagingDirDiff diff(HeadCommit()->tree());
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

}
