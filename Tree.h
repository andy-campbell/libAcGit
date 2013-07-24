#ifndef TREE_H
#define TREE_H

#include <git2.h>

#include "Gitexception.h"
#include "Repository.h"
#include "Treeentry.h"

namespace AcGit
{
    class Repository;
    class TreeEntry;

    class Tree
    {
        public:
            Tree(Repository *repo, git_tree *tree);
            ~Tree();

            Repository *getRepository();
            git_tree *internalTree();
            TreeEntry *entries();
            QList<TreeEntry *> getEntriesList();

            TreeEntry *getEntry(QString path);
    private:
            git_tree *tree;
            Repository *repo;
            QList<TreeEntry*> treeEntries;

            void setupEntries();

    };
}
#endif // TREE_H
