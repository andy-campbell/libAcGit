#ifndef BRANCH_H
#define BRANCH_H

#include <QString>

#include "Sha.h"

#include "libacgit_global.h"


namespace AcGit
{

    class LIBACGITBRANCH_EXPORT Branch : public Sha
    {
        public:
            Branch(QString branchName, Sha *sha, bool isLocal);
            virtual ~Branch();

            virtual QString getBranchName();
            virtual QString getRefName();
            virtual void setBranchName(QString branchName);


            virtual bool isLocal();
            virtual bool isRemote();

        private:
            QString branchName;
            bool isBranchLocal;


    };

}
#endif // BRANCH_H
