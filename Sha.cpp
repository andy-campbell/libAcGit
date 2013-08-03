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

#include "Sha.h"


namespace AcGit
{

Sha::Sha(const git_oid *shaPtr)
{

    this->sha = shaPtr;
}

Sha::Sha(const Sha *other)
    : sha(other->sha)
{
}

Sha::Sha(QString shaString)
{
    int len = qMin(shaString.length(), GIT_OID_HEXSZ);
    QByteArray shaArray = shaString.toLocal8Bit();
    git_oid *tempSha = (git_oid*)malloc(sizeof (git_oid));

    git_oid_fromstrn(tempSha, shaArray.constData(), len);
    sha = tempSha;
}

Sha::~Sha()
{
    //free(const_cast<git_oid *>(sha));
}

QByteArray Sha::toString()
{
    QByteArray shaString(GIT_OID_HEXSZ, Qt::Uninitialized);
    git_oid_fmt(shaString.data(), sha);
    return shaString;
}

bool Sha::operator ==(const Sha *sha)
{
    return git_oid_cmp( this->sha, sha->sha);
}

bool Sha::operator !=(const Sha *sha)
{
    return !(operator ==(sha));
}

const git_oid* Sha::raw()
{
    return sha;
}

}
