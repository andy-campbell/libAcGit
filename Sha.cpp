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
