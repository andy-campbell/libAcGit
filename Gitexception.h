#ifndef GITEXCEPTION_H
#define GITEXCEPTION_H

#include <exception>
#include <QByteArray>

namespace AcGit
{

    class GitException : public std::exception
    {
        public:
            GitException();

            ~GitException() throw();

            QByteArray exceptionMessage() const throw();

        private:
            QByteArray m;
    };

    int gitTest(int ret);

}

#endif // GITEXCEPTION_H
