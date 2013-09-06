#ifndef RESET_H
#define RESET_H

#include "IReset.h"
#include "Repository.h"
#include "Sha.h"

namespace AcGit
{

    class Reset : public IReset
    {
        public:
            Reset(Repository *repo);
            virtual ~Reset();

            virtual void resetToTarget(Sha *target, resetType type);
        private:
            git_reset_t getInternalType(resetType type);

            Repository *repo;
    };

}

#endif // RESET_H
