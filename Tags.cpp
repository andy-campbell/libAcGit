#include <QDebug>
#include <QStringList>
#include <QPair>
#include <QList>
#include <QByteArray>

#include "Tags.h"
#include "Gitexception.h"
#include "Tagscallback.h"

namespace AcGit
{

Tags::Tags(Repository *repo)
{
    this->repo = repo;
    tags = QList<Tag*>();
    populateTags();
}

Tags::~Tags()
{

}

void Tags::gitTagListLookupException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
}

void Tags::gitTagLookupException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
}


void Tags::populateTags()
{
    TagsCallback *allTags = new TagsCallback(repo);

    QList<QPair<QString, const git_oid *>> tagsInfo = allTags->getTags();

    QPair<QString, const git_oid*> tag;
    foreach (tag, tagsInfo)
    {
        git_tag *rawTag;

        git_tag_lookup(&rawTag, repo->getInternalRepo(), tag.second);

        tags.append(new Tag (tag.first, new Sha(tag.second), rawTag));
    }

}

QList<Tag *> Tags::listTags()
{
    return tags;
}

QStringList Tags::stringTagsList()
{
    QStringList stringTags;
    foreach (Tag *tag, tags)
    {
        stringTags.append(tag->name());
    }

    return stringTags;
}

void Tags::createTag(QString name, Sha *sha, Tagger *tagger, QString message)
{
    git_oid *objectSha = (git_oid*)malloc(sizeof (git_oid));
    git_oid *newSha;
    git_object *gitObject;

    git_oid_fromstr(objectSha, sha->toString().data());
    git_object_lookup(&gitObject, repo->getInternalRepo(), objectSha, GIT_OBJ_ANY);
    if (gitObject == nullptr)
    {
        qDebug() << "gitObject is null";
    }
    if (tagger->getRaw() == nullptr)
    {
        qDebug() << "tagger->getRaw() is null";
    }
    if(gitObject && tagger->getRaw())
        git_tag_create(newSha, repo->getInternalRepo(), name.toLocal8Bit(), gitObject,
                       tagger->getRaw(), message.toLocal8Bit(), 1);

    git_tag *newTag;
    git_tag_lookup(&newTag, repo->getInternalRepo(), newSha);

    tags.append(new Tag (name, new Sha(newSha), newTag));
}

void Tags::deleteTagException (GitException e)
{
    qCritical() << __func__ << ":" << __LINE__ << " " << e.exceptionMessage();
}

void Tags::deleteTag(Tag *tag)
{
    try
    {
        gitTest(git_tag_delete(repo->getInternalRepo(), tag->name().toLocal8Bit()));
        tags.removeOne(tag);
    }
    catch (GitException e)
    {
        deleteTagException(e);
    }
}

Tag *Tags::lookupTag(QString name)
{
    Tag *tagFound = nullptr;
    foreach(Tag *tag, tags)
    {
        if(tag->name().contains(name))
        {
            tagFound = tag;
            break;
        }
    }
    return tagFound;
}

}
