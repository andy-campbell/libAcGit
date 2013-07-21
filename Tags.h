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
