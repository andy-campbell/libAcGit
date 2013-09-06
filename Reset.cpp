#include <git2.h>

#include "Reset.h"
#include "Gitexception.h"
#include "Apiexception.h"

namespace AcGit {


Reset::Reset(AcGit::Repository *repo)
{
    this->repo = repo;
}

Reset::~Reset()
{

}

void Reset::resetToTarget(Sha *target, resetType type)
{

    git_reset_t internalType = getInternalType(type);
    git_object* targetObject;
    gitTest(git_object_lookup(&targetObject, repo->getInternalRepo(), target->raw(), GIT_OBJ_ANY));
    gitTest(git_reset(repo->getInternalRepo(), targetObject, internalType));
}

git_reset_t Reset::getInternalType(resetType type)
{
    git_reset_t internalType;
    if(type == SOFT)
        internalType = GIT_RESET_SOFT;

    else if(type == MIXED)
        internalType = GIT_RESET_MIXED;

    else if(type == HARD)
        internalType = GIT_RESET_HARD;

    else
        throw new ApiException("Not a valid reset type");

    return internalType;
}

}
