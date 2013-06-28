#include <QDebug>

#include "TreeDiff.h"
#include "Diffcallback.h"

namespace AcGit
{

TreeDiff::TreeDiff(Tree *treeOld, Tree *treeNew)
{
    try
    {
        git_repository* internalRepo = nullptr;
        if (treeOld && treeOld->getRepository() == treeNew->getRepository())
        {
            Repository *repository = treeNew->getRepository();
            internalRepo = repository->getInternalRepo();
        }
        else
        {
            Repository *repository = treeNew->getRepository();
            internalRepo = repository->getInternalRepo();
        }

        const git_diff_options options = GIT_DIFF_OPTIONS_INIT;

        if (treeOld == nullptr)
        {
            gitTest(git_diff_tree_to_tree(&differences, internalRepo, nullptr,
                                          treeNew->internalTree(), &options));
        }
        else
        {
            gitTest(git_diff_tree_to_tree(&differences, internalRepo, treeOld->internalTree(),
                                          treeNew->internalTree(), &options));
        }
    }
    catch (GitException e)
    {
        diffTreeToTreeException(e);
    }

    processDifferences();
}

TreeDiff::~TreeDiff()
{
    git_diff_list_free (differences);
}


void TreeDiff::diffTreeToTreeException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}



}
