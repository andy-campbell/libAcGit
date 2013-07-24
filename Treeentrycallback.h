#ifndef TREEENTRYCALLBACK_H
#define TREEENTRYCALLBACK_H

#include <git2.h>
#include <QString>
#include <QList>

#include "Treeentry.h"
#include "Gitexception.h"

namespace AcGit
{


    class TreeEntryCallback
    {
        public:
            TreeEntryCallback(git_tree *tree, Repository *repo);

            void addEntry(const char *root, const git_tree_entry *entry);
            QList<TreeEntry *> entries();

        private:
            QList<AcGit::TreeEntry*> treeEntries;
            Repository *repo;
    };

}
#endif // TREEENTRYCALLBACK_H
