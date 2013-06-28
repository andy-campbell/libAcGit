#include "Gitexception.h"
#include "git2.h"

namespace AcGit
{

GitException::GitException()
{
    m = giterr_last()->message;
}

GitException::~GitException() throw()
{
}

QByteArray GitException::exceptionMessage() const throw()
{
    return m;
}

int gitTest(int ret)
{
    if (ret < 0) {
        throw GitException();
    }
    return ret;
}

}
