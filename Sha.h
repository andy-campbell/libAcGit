#ifndef SHA_H
#define SHA_H

#include <QByteArray>
#include <QString>
#include <git2.h>

#include "libacgit_global.h"

namespace AcGit
{

    class LIBACGITSHA_EXPORT Sha
    {
    public:
        Sha(const git_oid *shaPtr = nullptr);
        Sha(const Sha *other);
        Sha(QString shaString);

        virtual ~Sha();

        QByteArray toString();

        bool operator==(const Sha *sha);
        bool operator!=(const Sha *sha);

        // for internal use only
        const git_oid *raw();

    protected:
        const git_oid *sha;
    };
}
#endif // SHA_H
