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
        TreeEntry *treeEntry = new TreeEntry(tree, path, repo);
        treeEntries.push_back(treeEntry);
    }
}

TreeEntryCallback::TreeEntryCallback(git_tree *tree, Repository *repo)
{
    this->repo = repo;
    this->tree = tree;
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
