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
    //TODO optomize
    foreach (Branch *branch, localBranches)
    {
        if (branch->getRefName().compare(name) == 0)
        {
            return branch;
        }
    }

    foreach (Branch *branch, remoteBranches)
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
    foreach (Branch *branch, localBranches)
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
    //stub
    qDebug() << "stub " << __FUNCTION__;
}

}
