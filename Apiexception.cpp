#include "Apiexception.h"

namespace AcGit
{

ApiException::ApiException(QString reason)
{
    this->reason = reason;
}

QString ApiException::getMessage()
{
    return reason;
}

ApiException::~ApiException() throw()
{
}


}
