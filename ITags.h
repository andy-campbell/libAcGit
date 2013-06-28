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

    };

    inline ITags::~ITags() {}
}

#endif // ITAGS_H
