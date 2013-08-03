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

#ifndef TAGS_H
#define TAGS_H

#include <QStringList>

#include "ITags.h"
#include "Tag.h"
#include "Repository.h"
#include "Tagger.h"

namespace AcGit
{

    class Tags : public ITags
    {
        public:
            Tags(Repository *repo);

            ~Tags();

            virtual QList<Tag *> listTags();

            virtual QStringList stringTagsList();

            virtual void createTag(QString name, Sha *sha, Tagger *tagger, QString message);

            virtual void deleteTag(Tag *tag);

            virtual Tag *lookupTag(QString name);

            virtual QList<Tag *> lookupTag(Sha *sha);

    private:
            Repository *repo;
            QList<Tag*> tags;


            void populateTags();
            void gitTagListLookupException(GitException e);
            void gitTagLookupException(GitException e);
            void deleteTagException(GitException e);

    };

}
#endif // TAGS_H
