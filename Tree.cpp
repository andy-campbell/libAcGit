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
        {
            return entry;
        }
    }
    return nullptr;
}

}
