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
#include <QFile>
#include <QFileInfo>

#include "Treeentry.h"
#include "Gitexception.h"

namespace AcGit
{


TreeEntry::TreeEntry(const git_tree *root, QString path, Repository *repo)
{
    this->repo = repo;
    this->root = root;
    this->path = path;
}

QString TreeEntry::fullPath() const
{
    return path;
}

QString TreeEntry::fileName()
{
    QFile file(path);
    QFileInfo fileInfo(file.fileName());
    return fileInfo.fileName();
}

unsigned int TreeEntry::attributes() const
{
    git_tree_entry *entry;
    gitTest(git_tree_entry_bypath(&entry, root, fullPath().toLocal8Bit()));

    unsigned int attributes = git_tree_entry_filemode(entry);
    git_tree_entry_free(entry);

    return attributes;
}

Blob* TreeEntry::fileBlob()
{
    git_tree_entry *entry;
    git_object* treeEntryObject;
    try
    {
        gitTest(git_tree_entry_bypath(&entry, root, fullPath().toLocal8Bit()));

        gitTest(git_tree_entry_to_object(&treeEntryObject, repo->getInternalRepo(), entry));
    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
    }

    return new Blob(treeEntryObject);
}

}
