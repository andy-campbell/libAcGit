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
