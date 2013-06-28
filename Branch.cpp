#include "Branch.h"

namespace AcGit
{


Branch::Branch(QString branchName, Sha *sha, bool isLocal)
    : Sha(sha), isBranchLocal(isLocal)
{
    this->branchName = branchName;
}

Branch::~Branch()
{
}


QString Branch::getBranchName()
{
    return branchName;
}

QString Branch::getRefName()
{
    QString refName;
    if (isLocal())
    {
        refName = "refs/heads/" + branchName;
    }
    else if (isRemote())
    {
        refName = "refs/remotes/" + branchName;
    }
    return refName;
}

void Branch::setBranchName(QString branchName)
{
    this->branchName = branchName;
}


bool Branch::isLocal()
{
    return isBranchLocal;
}

bool Branch::isRemote()
{
    return !isLocal();
}

}
