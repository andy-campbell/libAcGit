/*

Copyright (c) 2013 Andrew Campbell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

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

            git_tree *tree;
    };

}
#endif // TREEENTRYCALLBACK_H
