#include "Credintials.h"

namespace  AcGit {


Credintials::Credintials(QString username, QString password)
{
    this->username = username;
    this->password = password;
}

QString Credintials::getUsername()
{
    return username;
}

QString Credintials::getPassword()
{
    return password;
}

}
