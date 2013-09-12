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


#ifndef BRANCHES_H
#define BRANCHES_H

#include <QList>
#include <git2.h>

#include "IBranches.h"
#include "Branch.h"

namespace AcGit
{

    class Branches : public IBranches
    {
        public:
            Branches(git_repository* repo);

            virtual QList<Branch*> getAllBranches();

            virtual QList<Branch*> getRemoteBranches();

            virtual QList<Branch*> getLocalBranches();

            virtual QStringList branchNamesList();

            virtual void createNewBranch(QString branchName, Commit* commit, int force = 1);

            virtual void deleteBranch(Branch *branch);

            virtual void moveBranch(Branch *branch, QString newBranchName, int force = 1);

            virtual Branch *lookupBranch(QString name);

            virtual QList<Branch *> lookupBranch(Sha *sha);

            virtual QString currentBranch();

            void updateActiveBranch();
    private:

            void createLocalBranch(QString branchName);
            void createRemoteBranch(QString branchName);
            void populateRemoteBranches();
            void populateLocalBranches();

            QList<Branch*> localBranches;
            QList<Branch*> remoteBranches;

            git_repository *repo;

    };

}

#endif // BRANCHES_H
