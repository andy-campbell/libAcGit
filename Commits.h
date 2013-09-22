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
            Commits (Repository *repo, Sha *shaToWalkFrom, SortMode mode = SORT_TOPOLOGICAL);

            virtual QList<Commit *> *getAllCommits();

            virtual void setSortingMode(SortMode mode);

            Commit* lookupCommit(Sha *sha);

            void updateCommits(Sha *shaToWalkFrom);
    private:
            Repository *repo;
            SortMode currentSortMode;
            QList<Commit *> *CommitsList;
            QMap<QString ,Commit *> lookupMap;


            void generateGraph();
            void addHeadCommit();
            void startRevWalk(git_revwalk *walker);
            void setStartCommit(git_revwalk *walker, Sha *sha);
            void gitRevWalkPushHeadException(GitException e);
            void gitRevWalkNextException(GitException e);
            void applySortMode(git_revwalk *walker, bool reverse);
            void revNewWalkerError(GitException e);

    };
}
#endif // COMMITS_H
