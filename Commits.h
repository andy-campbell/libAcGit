#ifndef COMMITS_H
#define COMMITS_H

#include <QList>
#include <QMap>

#include "ICommits.h"
#include "Repository.h"
#include "Gitexception.h"

namespace AcGit
{

    class Commits : public ICommits
    {
        public:
            Commits (Repository *repo, SortMode mode = SORT_TOPOLOGICAL);

            virtual QList<Commit *> *getAllCommits();

            virtual void setSortingMode(SortMode mode);

            Commit* lookupCommit(Sha *sha);
        private:
            Repository *repo;
            SortMode currentSortMode;
            QList<Commit *> *CommitsList;
            QMap<QString ,Commit *> lookupMap;


            void addHeadCommit();
            void startRevWalk(git_revwalk *walker);
            void setStartCommit(git_revwalk *walker);
            void gitRevWalkPushHeadException(GitException e);
            void gitRevWalkNextException(GitException e);
            void applySortMode(git_revwalk *walker);
            void revNewWalkerError(GitException e);
    };
}
#endif // COMMITS_H
