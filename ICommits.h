#ifndef ICOMMITS_H
#define ICOMMITS_H

#include <QList>

#include "Commit.h"
#include "Sha.h"

namespace AcGit
{
    class Commit;
    class Sha;

    class ICommits
    {
    public:
        virtual ~ICommits() = 0;

        virtual QList<Commit*> *getAllCommits () = 0;

        typedef enum SortMode
        {
            SORT_NONE,
            SORT_TOPOLOGICAL,
            SORT_TIME,
            SORT_REVERSE
        } SortMode;

        virtual void setSortingMode(SortMode method) = 0;

        virtual Commit* lookupCommit(Sha *sha) = 0;


    };

    inline ICommits::~ICommits() {}
}

#endif // ICOMMITS_H
