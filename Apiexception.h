#ifndef APIEXCEPTION_H
#define APIEXCEPTION_H

#include <QString>

namespace AcGit {


    class ApiException
    {
    public:
        ApiException(QString reason);
        ~ApiException() throw();

        QString getMessage();

    private:
        QString reason;
    };

}
#endif // APIEXCEPTION_H
