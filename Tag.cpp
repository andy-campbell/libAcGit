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
