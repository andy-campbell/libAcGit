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

#include "Branches.h"
#include "Branchescallback.h"
#include "Gitexception.h"

namespace AcGit
{

Branches::Branches(git_repository *repo)
{
    this->repo = repo;

    populateLocalBranches();

    populateRemoteBranches();
}

QList<Branch*> Branches::getAllBranches()
{
    QList<Branch*> allBranches;

    allBranches.append(localBranches);
    allBranches.append(remoteBranches);

    return allBranches;
}

QList<Branch *> Branches::getRemoteBranches()
{
    return remoteBranches;
}

QList<Branch *> Branches::getLocalBranches()
{
    return localBranches;
}

QStringList Branches::branchNamesList()
{
    QStringList branchNames;
    foreach (Branch* branch, getAllBranches())
    {
        branchNames.append(branch->getBranchName());
    }
    return branchNames;
}

void Branches::createNewBranch(QString branchName, Commit* commit, int force)
{
    try
    {
        git_reference *ref;
        gitTest(git_branch_create(&ref, repo, branchName.toLocal8Bit(),commit->rawCommit(), force));

        createLocalBranch (branchName);
    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
    }
}

void Branches::deleteBranch(Branch *branch)
{
    try
    {
        git_reference *ref;

        QString refName = branch->getRefName();
        git_reference_lookup(&ref, repo, refName.toLocal8Bit());

        if (ref != nullptr)
        {
            git_branch_delete(ref);
        }

        if (branch->isLocal())
        {
            localBranches.removeOne(branch);
        }
        else
        {
            remoteBranches.removeOne(branch);
        }
        delete branch;
    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
    }
}

void Branches::moveBranch(Branch *branch, QString newBranchName, int force)
{
    try
    {
        git_reference *curRef;
        QString curRefName = branch->getRefName();
        gitTest(git_reference_lookup(&curRef, repo, curRefName.toLocal8Bit()));

        git_reference *newRef;
        gitTest(git_branch_move(&newRef, curRef, newBranchName.toLocal8Bit(), force));
        branch->setBranchName(newBranchName);

    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
    }

}

Branch *Branches::lookupBranch(QString name)
{
    foreach (Branch *branch, getAllBranches())
    {
        if (branch->getRefName().compare(name) == 0)
        {
            return branch;
        }
    }

    return nullptr;
}

QList<Branch *> Branches::lookupBranch(Sha *sha)
{
    QList<Branch *> matchingBranches;
    foreach (Branch *branch, getAllBranches())
    {
        if (branch->toString().contains(sha->toString()))
        {
            matchingBranches.append(branch);

        }
    }

    return matchingBranches;
}

void Branches::createLocalBranch(QString branchName)
{
    git_oid sha;
    Branch *branch = nullptr;
    git_oid *shaCopy = (git_oid *)malloc (sizeof(struct git_oid));
    QString refName = "refs/heads/" + branchName;
    gitTest( git_reference_name_to_id(&sha, repo, refName.toLocal8Bit()));

    git_oid_cpy(shaCopy, &sha);
    branch = new Branch(branchName, new Sha(shaCopy), true);

    localBranches << branch;
}

void Branches::createRemoteBranch(QString branchName)
{
    git_oid sha;
    Branch *branch = nullptr;

    git_oid *shaCopy = (git_oid *)malloc (sizeof(struct git_oid));
    QString refName = "refs/remotes/" + branchName;
    gitTest( git_reference_name_to_id(&sha, repo, refName.toLocal8Bit()));

    git_oid_cpy(shaCopy, &sha);
    branch = new Branch(branchName, new Sha(shaCopy), false);

    remoteBranches << branch;
}


void Branches::populateLocalBranches()
{
    BranchesCallBack branches(repo, BranchesCallBack::BRANCH_LOCAL);

    foreach (QString branchName, branches.getBranches())
    {
        try
        {
            createLocalBranch(branchName);
        }
        catch (GitException e)
        {
            qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
        }
    }
}

void Branches::populateRemoteBranches()
{
    BranchesCallBack branches(repo, BranchesCallBack::BRANCH_REMOTE);

    foreach (QString branchName, branches.getBranches())
    {
        try
        {
            createRemoteBranch(branchName);
        }
        catch (GitException e)
        {
            qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
        }
    }
}

QString Branches::currentBranch()
{
    git_reference *headRef;
    QString currentBranchName = "No Branch";
    try
    {
        int refType = GIT_REF_INVALID;
        gitTest(git_repository_head (&headRef, repo));
        refType = git_reference_type(headRef);
        if (refType == GIT_REF_SYMBOLIC)
        {
            currentBranchName = QString(git_reference_symbolic_target(headRef));
        }
        else if (refType == GIT_REF_OID)
        {
            currentBranchName = QString(git_reference_name (headRef));
        }
    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
    }

    return currentBranchName;
}

void Branches::updateActiveBranch()
{
    QString activeBranchName = currentBranch();

    Branch* activeBranch = lookupBranch(activeBranchName);
    activeBranch->getRefName();
    localBranches.removeOne(activeBranch);

    delete activeBranch;

    activeBranchName = activeBranchName.right(activeBranchName.length() - activeBranchName.lastIndexOf('/') - 1);
    createLocalBranch(activeBranchName);
}

}
