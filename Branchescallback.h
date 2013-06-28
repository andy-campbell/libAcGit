#ifndef BRANCHESCALLBACK_H
#define BRANCHESCALLBACK_H

#include <QStringList>
#include <git2.h>

class BranchesCallBack
{
    public:
        typedef enum BranchType {
            BRANCH_LOCAL,
            BRANCH_REMOTE
        } BranchType;

        BranchesCallBack(git_repository *repo, BranchType type);

        QStringList getBranches();

        void addBranch(const char *branchName);
private:
        QStringList branches;
        BranchType type;



};

#endif // BRANCHESCALLBACK_H
