#ifndef LIBACGIT_H
#define LIBACGIT_H

#include "libacgit_global.h"

#include <QString>
#include <git2.h>
#include <QtCore/qglobal.h>

#include "ICommits.h"
#include "IBranches.h"
#include "ITags.h"

#if defined(LIBACGIT_LIBRARY)
#  define LIBACGITREPOSITORY_EXPORT Q_DECL_EXPORT
#else
#  define LIBACGITREPOSITORY_EXPORT Q_DECL_IMPORT
#endif

namespace AcGit
{
    class IBranches;
    class ICommits;
    class Commit;
    class ITags;

    class LIBACGITREPOSITORY_EXPORT Repository
    {

        public:
            Repository(QString path);

            ~Repository();

            IBranches *BranchAgent();

            git_repository *getInternalRepo();
            ICommits *CommitsAgent();
            ITags *TagsAgent();
            Commit *HeadCommit();
        private:
            git_repository *repo;
            ICommits *commits;
            IBranches *branches;
            ITags *tags;
    };

}
#endif // LIBACGIT_H
