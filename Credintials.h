#ifndef CREDINTIALS_H
#define CREDINTIALS_H

#include <QString>

namespace AcGit
{

class Credintials
{
public:
    Credintials(QString username, QString password);

    QString getUsername();
    QString getPassword();
private:
    QString username;
    QString password;
};


}
#endif // CREDINTIALS_H
