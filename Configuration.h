#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include <git2.h>

#include "Gitexception.h"
#include "Repository.h"

namespace AcGit
{
    class Repository;
    class Configuration
    {
    public:
        Configuration(Repository *repo);

        QString userEmail();

        QString userName();

        QString getString(QString configurationOption);

    private:
        void openConfigutation();
        Repository *repo;
        git_config *cfg;
    };
}
#endif // CONFIGURATION_H
