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

    repo->Update();
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
