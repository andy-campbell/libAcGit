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
