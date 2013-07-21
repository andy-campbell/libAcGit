#include "Gitexception.h"
#include "git2.h"

namespace AcGit
{

GitException::GitException(int ret)
{
    m = giterr_last()->message;
    returnVal = ret;
}

GitException::~GitException() throw()
{
}

QByteArray GitException::exceptionMessage() const throw()
{
    return m;
}

int GitException::returnValue()
{
    int returnVal;
}

int gitTest(int ret)
{
    if (ret < 0) {
        throw GitException(ret);
    }
    return ret;
}

}
