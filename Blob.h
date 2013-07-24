#ifndef BLOB_H
#define BLOB_H

#include <git2.h>
#include <QString>

namespace AcGit
{
    class Blob
    {
        public:
            Blob(git_blob *blob);

            QString contents() const;

    private:
            int rawBlobSize() const;
            const void *rawBlob() const;

            const git_blob *blob;
    };


}


#endif // BLOB_H
