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

#ifndef ITAGS_H
#define ITAGS_H

#include <QString>
#include <QStringList>

#include "Sha.h"
#include "Tag.h"
#include "Tagger.h"

namespace AcGit {

    class ITags
    {
        public:
            virtual ~ITags () = 0;

            virtual QList<Tag*> listTags() = 0;

            virtual QStringList stringTagsList() = 0;

            virtual void createTag(QString name, Sha *sha, Tagger *tagger, QString message) = 0;

            virtual void deleteTag(Tag *tag) = 0;

            virtual Tag* lookupTag(QString name) = 0;

            virtual QList<Tag*> lookupTag(Sha *sha) = 0;

    };

    inline ITags::~ITags() {}
}

#endif // ITAGS_H
