#include <QDebug>

#include "Treeentry.h"
#include "Gitexception.h"

namespace AcGit
{


TreeEntry::TreeEntry(const git_tree_entry *treeEntry, QString path, Repository *repo)
{
    this->repo = repo;
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

Blob TreeEntry::fileBlob()
{
    git_object* treeEntryObject;
    try
    {
        gitTest(git_tree_entry_to_object(&treeEntryObject, repo->getInternalRepo(), treeEntry));
    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
    }

    Blob entryBlob((git_blob*)treeEntryObject);
    return entryBlob;
}

}
