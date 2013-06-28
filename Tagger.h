#ifndef TAGGER_H
#define TAGGER_H

#include <git2.h>
#include <QString>
#include <QDateTime>

namespace AcGit
{

    class Tagger
    {
    public:
        Tagger(QString name, QString email);
        Tagger(const git_signature *signature);

        ~Tagger();

        QString name() const;
        QString email() const;
        QDateTime date() const;

        const git_signature *getRaw();
    private:
        const git_signature *signature;
    };
}
#endif // TAGGER_H
