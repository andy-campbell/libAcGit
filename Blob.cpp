#include "Blob.h"

namespace AcGit
{

Blob::Blob(git_blob *blob)
{
    this->blob = blob;
}


QString Blob::contents() const
{
    QString content(QByteArray::fromRawData( static_cast<const char *>(rawBlob()), rawBlobSize()));
    return content;
}

const void *Blob::rawBlob() const
{
    return git_blob_rawcontent(blob);
}

int Blob::rawBlobSize() const
{
    return git_blob_rawsize(blob);
}

}
