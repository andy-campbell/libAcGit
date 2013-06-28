#ifndef WORKINGDIRDIFF_H
#define WORKINGDIRDIFF_H

#include "Diff.h"
#include "Tree.h"
#include "Gitexception.h"

namespace AcGit
{

    class WorkingDirDiff : public Diff
    {
    public:
        WorkingDirDiff(Tree *headCommitTree);
        ~WorkingDirDiff();

    private:
        void workingTreeDiffException(GitException e);

    };

}

#endif // WORKINGDIRDIFF_H
