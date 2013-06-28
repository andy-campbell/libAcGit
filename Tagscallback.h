#ifndef TAGSCALLBACK_H
#define TAGSCALLBACK_H

#include <QString>
#include <QList>
#include <QString>

#include "Repository.h"

namespace AcGit
{

    class TagsCallback
    {
        public:
            TagsCallback(Repository *repo);
            ~TagsCallback();

            void addTag(const char *tagName, const git_oid *sha);

            QList<QPair<QString, const git_oid *>> getTags();

    private:
            QList <QPair <QString, const git_oid*>> tagsList;

            void gitTagForEachException(GitException e);
    };

}
#endif // TAGSCALLBACK_H
