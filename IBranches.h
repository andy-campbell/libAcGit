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


#ifndef IBRANCHES_H
#define IBRANCHES_H

#include <QString>
#include <QStringList>

#include "libacgit_global.h"

#include "Repository.h"
#include "Branch.h"
#include "Sha.h"
#include "Commit.h"

namespace AcGit
{
    class Branch;
    class Commit;

    class LIBACGITBRANCHES_EXPORT IBranches
    {
        public:
            virtual ~IBranches();

            virtual QList<Branch*> getAllBranches() = 0;

            virtual QList<Branch*> getRemoteBranches() = 0;

            virtual QList<Branch*> getLocalBranches() = 0;

            virtual QStringList branchNamesList() = 0;

            virtual void createNewBranch(QString branchName, Commit *commit, int force = 1) = 0;

            virtual void deleteBranch(Branch *branch) = 0;

            virtual void moveBranch(Branch *branch, QString newBranchName, int force = 1) = 0;

            virtual Branch *lookupBranch(QString name) = 0;

            virtual QList<Branch *> lookupBranch(Sha *sha) = 0;
    };

    inline IBranches::~IBranches() {}
}
#endif // IBRANCHES_H
