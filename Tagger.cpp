#include "Tagger.h"

namespace AcGit
{

Tagger::Tagger(QString name, QString email)
{
    git_signature *newSignature;
    git_signature_now(&newSignature, name.toLocal8Bit().data(), email.toLocal8Bit().data());
    this->signature = newSignature;
}

Tagger::Tagger(const git_signature *signature)
{
    this->signature = signature;
}

Tagger::~Tagger()
{
}

QString Tagger::name() const
{
    return QString(signature->name);
}

QString Tagger::email() const
{
    return QString(signature->email);
}

QDateTime Tagger::date() const
{
    QDateTime dateTime;
    dateTime.setTime_t(signature->when.time);
    dateTime.setUtcOffset(signature->when.offset);
    return dateTime;
}

const git_signature *Tagger::getRaw()
{
    return signature;
}

}
