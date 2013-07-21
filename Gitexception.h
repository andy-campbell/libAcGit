#ifndef GITEXCEPTION_H
#define GITEXCEPTION_H

#include <exception>
#include <QByteArray>

namespace AcGit
{

    class GitException : public std::exception
    {
        public:
            GitException(int ret);

            ~GitException() throw();

            QByteArray exceptionMessage() const throw();

            int returnValue();

        private:
            QByteArray m;
            int returnVal;
    };

    int gitTest(int ret);

}

#endif // GITEXCEPTION_H
