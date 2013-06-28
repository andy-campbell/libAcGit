#include <QDebug>

#include "Treeentrycallback.h"
#include "Treeentry.h"

namespace AcGit
{

extern "C" int addEntriesCallBack(const char *root, const git_tree_entry *entry, void *payload)
{

    reinterpret_cast<TreeEntryCallback*> (payload)->addEntry(root, entry);
    // we want to continue looping so return 0
    return 0;
}


void TreeEntryCallback::addEntry(const char* root, const git_tree_entry *entry)
{
    git_otype type = git_tree_entry_type(entry);
    if (type == GIT_OBJ_BLOB)
    {
        QString path = QString(root) + QString (git_tree_entry_name(entry));
        TreeEntry *treeEntry = new TreeEntry(entry, path);
        treeEntries.push_back(treeEntry);
    }
}

TreeEntryCallback::TreeEntryCallback(git_tree *tree)
{
    try
    {
        gitTest(git_tree_walk(tree, GIT_TREEWALK_PRE, addEntriesCallBack, this));
    }
    catch (GitException e)
    {
        qCritical() << e.what();
    }
}

QList<TreeEntry*> TreeEntryCallback::entries()
{
    return treeEntries;
}

}
