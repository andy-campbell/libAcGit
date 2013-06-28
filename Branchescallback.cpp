#include <git2.h>

#include "Branchescallback.h"

extern "C" int branchesCallBack(const char *branchName, git_branch_t branchType,
                                void *payload)
{
    reinterpret_cast<BranchesCallBack*> (payload)->addBranch(branchName);

    // we want to continue looping so return 0
    return 0;
}

BranchesCallBack::BranchesCallBack(git_repository *repo, BranchType type)
{
    if (type == BRANCH_REMOTE)
        git_branch_foreach(repo, GIT_BRANCH_REMOTE, branchesCallBack, (void*)this);
    else
        git_branch_foreach(repo, GIT_BRANCH_LOCAL, branchesCallBack, (void*)this);
}

void BranchesCallBack::addBranch(const char *branchName)
{
    branches << QString(branchName);
}

QStringList BranchesCallBack::getBranches()
{
    return branches;
}


