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

#include "Blob.h"
#include "Gitexception.h"

namespace AcGit
{

Blob::Blob(git_object *blob)
{
    if (git_object_type(blob) == GIT_OBJ_BLOB)
    {
        this->blob = (git_blob*)blob;
    }
    else
    {
        throw GitException(255);
    }
}

Blob::~Blob()
{
    git_object_free((git_object*)blob);
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
