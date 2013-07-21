#include <QDebug>
#include "Tag.h"
#include "Gitexception.h"

namespace AcGit
{

Tag::Tag(QString name, Sha *sha, git_tag *tag)
    : Sha (sha)
{
    this->tag = tag;
    qDebug() << tagName;
    tagName = name;
}


QString Tag::name()
{
    return tagName.right(tagName.size() - tagName.lastIndexOf('/') - 1);
}

QString Tag::refName()
{
    return tagName;
}

QString Tag::message()
{
    QString message;

    if (tag)
        message = QString(git_tag_message(tag));

    return message;
}

Tagger* Tag::getTagger()
{
    Tagger *tagger = nullptr;
    if (tag)
    {
        const git_signature *sig = git_tag_tagger(tag);
        tagger  = new Tagger(sig);
    }
    return tagger;
}

const git_tag *Tag::rawTag()
{
    return tag;
}

}
