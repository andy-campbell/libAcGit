#ifndef TREEENTRY_H
#define TREEENTRY_H

#include <QString>
#include <git2.h>

#include "Sha.h"
#include "Blob.h"
#include "Repository.h"

namespace AcGit
{
    class Repository;

    class TreeEntry : public Sha
    {
        public:
            TreeEntry(const git_tree_entry *treeEntry, QString path, Repository *repo);

            QString fileName() const;

            QString fullPath() const;

            unsigned int attributes() const;

            Blob fileBlob();
        private:
            const git_tree_entry *treeEntry;
            QString path;
            Repository *repo;

    };

}

#endif // TREEENTRY_H
