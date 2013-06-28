#ifndef TREEENTRY_H
#define TREEENTRY_H

#include <QString>
#include <git2.h>

#include "Sha.h"

namespace AcGit
{

    class TreeEntry : public Sha
    {
        public:
            TreeEntry(const git_tree_entry *treeEntry, QString path);

            QString fileName() const;

            QString fullPath() const;

            unsigned int attributes() const;
        private:
            const git_tree_entry *treeEntry;
            QString path;

    };

}

#endif // TREEENTRY_H
