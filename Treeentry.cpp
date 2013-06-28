#include "Treeentry.h"

namespace AcGit
{


TreeEntry::TreeEntry(const git_tree_entry *treeEntry, QString path)
{
    this->treeEntry = treeEntry;
    this->path = path;
}

QString TreeEntry::fileName() const
{
    const char *entryName = git_tree_entry_name(treeEntry);

    QString fileName(entryName);

    return fileName;
}

QString TreeEntry::fullPath() const
{
    return path;
}

unsigned int TreeEntry::attributes() const
{
    return git_tree_entry_filemode(treeEntry);
}

}
