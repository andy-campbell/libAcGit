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
        tagsMap.insert(tag.first, tags.last());
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
    git_oid newSha;
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
    {
        git_tag_create(&newSha, repo->getInternalRepo(), name.toLocal8Bit(), gitObject,
                       tagger->getRaw(), message.toLocal8Bit(), 0);
    }

    git_tag *newTag;
    git_tag_lookup(&newTag, repo->getInternalRepo(), &newSha);

    tags.append(new Tag (name, new Sha(&newSha), newTag));

    foreach (Tag *tag, tags)
    {
        qDebug() << "tagName" << tag->name();
    }
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

QList<Tag *> Tags::lookupTag(Sha *sha)
{
    QList<Tag *> matchingTags;
    foreach (Tag *tag, tags)
    {
        bool matchFound = false;

        if (tag->toString().contains(sha->toString()))
        {
            matchFound = true;
        }

        if (tag->rawTag())
        {
            Sha tagTarget(git_tag_target_id(tag->rawTag()));
            if (tagTarget.toString().contains(sha->toString()) == true)
            {
                matchFound = true;
            }
        }

        if (matchFound)
        {
            matchingTags.append(tag);
        }
    }
    return matchingTags;
}

void Tags::upateTags()
{
    git_strarray listOfTags;
    git_tag_list(&listOfTags, repo->getInternalRepo());

    for(int index = 0; index < listOfTags.count; index++)
    {
        QString tagName(listOfTags.strings[0]);
        QMap<QString ,Tag *>::iterator tagsItor;
        tagsItor = tagsMap.find(tagName);
        if (tagsItor == tagsMap.end())
        {
            git_oid sha;
            git_tag *rawTag;
            QString fullReference = "refs/tags/" + tagName;
            gitTest(git_reference_name_to_id(&sha, repo->getInternalRepo(), fullReference.toLocal8Bit()));
            git_oid *shaCopy = (git_oid *)malloc (sizeof(struct git_oid));
            git_oid_cpy(shaCopy, &sha);
            git_tag_lookup(&rawTag, repo->getInternalRepo(), &sha);

            tags.append(new Tag (tagName, new Sha(shaCopy), rawTag));
            tagsMap.insert(tagName, tags.last());
        }


    }
}

}
