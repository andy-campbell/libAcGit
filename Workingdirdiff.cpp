
#include <QDebug>

#include "Workingdirdiff.h"
#include "Commit.h"

namespace AcGit
{

WorkingDirDiff::WorkingDirDiff(Tree *headCommitTree)
{
    // get the diff
    const git_diff_options options = GIT_DIFF_OPTIONS_INIT;
    Repository *repo = headCommitTree->getRepository();


    try
    {
        gitTest(git_diff_tree_to_workdir (&differences, repo->getInternalRepo(),
                                          headCommitTree->internalTree() , &options));
    }
    catch(GitException e)
    {
        workingTreeDiffException(e);
    }

    processDifferences();
}

WorkingDirDiff::~WorkingDirDiff()
{

}

void WorkingDirDiff::workingTreeDiffException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}

}
