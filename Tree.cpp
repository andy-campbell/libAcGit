#include "Tree.h"
#include "Treeentrycallback.h"


namespace AcGit {


Tree::Tree(Repository *repo, git_tree *tree)
{
    this->repo = repo;
    this->tree = tree;

    setupEntries();
}


Tree::~Tree()
{
}

void Tree::setupEntries()
{
    TreeEntryCallback entries(tree, repo);

    treeEntries = entries.entries();
}

Repository *Tree::getRepository()
{
    return repo;
}

git_tree *Tree::internalTree()
{
    return tree;
}

QList<TreeEntry*> Tree::getEntriesList()
{
    return treeEntries;
}

TreeEntry* Tree::getEntry(QString path)
{
    foreach(TreeEntry *entry, treeEntries)
    {
        if (entry->fullPath().compare(path) == 0)
            return entry;
    }
}

}
