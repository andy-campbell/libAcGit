#ifndef TREEDIFF_H
#define TREEDIFF_H

#include <QStringList>
#include <QString>

#include "Tree.h"
#include "Diff.h"
#include "Gitexception.h"

namespace AcGit {

    class TreeDiff : public Diff
    {
        public:
            TreeDiff(Tree *treeOld, Tree *treeNew);

            ~TreeDiff();

         private:
            void diffTreeToTreeException(GitException e);

    };

}
#endif // DIFF_H
