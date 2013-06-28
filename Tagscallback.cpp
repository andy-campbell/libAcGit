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
