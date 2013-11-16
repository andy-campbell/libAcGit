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

#ifndef LIBACGIT_H
#define LIBACGIT_H

#include "libacgit_global.h"

#include <QString>
#include <git2.h>
#include <QtCore/qglobal.h>

#include "ICommits.h"
#include "IBranches.h"
#include "ITags.h"
#include "IReset.h"
#include "Configuration.h"

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
    class IReset;
    class Configuration;
    class Diff;

    class LIBACGITREPOSITORY_EXPORT Repository
    {

        public:
            Repository(QString path);
            Repository(git_repository *internalRepo);

            ~Repository();

            IBranches *BranchAgent();

            git_repository *getInternalRepo();
            ICommits *CommitsAgent();
            ITags *TagsAgent();
            Configuration *ConfigurationAgent();
            Commit *HeadCommit();
            Commit *ActiveBranchCommit();
            IReset *ResetAgent();

            bool HasWorkingTreeChanges();
            bool HasStagingDirChanges();

            QString GitTopLevelDirectory();
            int AheadBehind(Sha *from, Sha *to);
            void Update();
    private:
            git_repository *repo;
            ICommits *commits;
            IBranches *branches;
            ITags *tags;
            IReset *reset;
            Configuration *config;
            bool hasWorkingTreeChanges;
            bool hasChanges(Diff &diff);
            void initaliseRepo();
            Sha *getRepositoryHead();
            Sha *branchHead;
    };

}
#endif // LIBACGIT_H
