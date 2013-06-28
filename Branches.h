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
    private:

            void createLocalBranch(QString branchName);
            void populateRemoteBranches();
            void populateLocalBranches();

            QList<Branch*> localBranches;
            QList<Branch*> remoteBranches;

            git_repository *repo;

    };

}

#endif // BRANCHES_H
