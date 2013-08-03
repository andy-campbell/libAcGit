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

#include <git2.h>
#include <QDebug>
#include <QByteArray>

#include "Tagscallback.h"
#include "Gitexception.h"
#include "Repository.h"

namespace AcGit
{

extern "C" int tagsCallBack(const char *tagName, git_oid *sha, void *payload)
{
    reinterpret_cast<TagsCallback*> (payload)->addTag(tagName, sha);

    // we want to continue looping so return 0
    return 0;
}

TagsCallback::TagsCallback(Repository *repo)
{
    try
    {
        tagsList = QList <QPair <QString, const git_oid*>>();
        gitTest(git_tag_foreach(repo->getInternalRepo(),tagsCallBack ,this));
    }
    catch (GitException e)
    {
        gitTagForEachException (e);
    }
}

TagsCallback::~TagsCallback()
{
}

void TagsCallback::addTag(const char *tagName, const git_oid *sha)
{
    QPair<QString, const git_oid*> tagInfo;

    tagInfo.first = QString(tagName);
    git_oid *shaCopy = (git_oid *)malloc (sizeof(struct git_oid));
    git_oid_cpy(shaCopy, sha);
    tagInfo.second = shaCopy;

    tagsList.append(tagInfo);
}

QList<QPair<QString, const git_oid *> > TagsCallback::getTags()
{
    return tagsList;
}

void TagsCallback::gitTagForEachException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}

}
