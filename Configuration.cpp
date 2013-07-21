#include <QDebug>

#include "Configuration.h"

namespace AcGit
{

Configuration::Configuration(Repository *repo)
{
    this->repo = repo;
    openConfigutation();
}

void Configuration::openConfigutation()
{
    git_config_new(&cfg);

    // add local configuration
    const char *path = git_repository_path(repo->getInternalRepo());
    QString configPath = QString(path);
    configPath += ".git/config";
    git_config_add_file_ondisk(cfg, configPath.toLocal8Bit(), GIT_CONFIG_LEVEL_LOCAL, 0);

    // add global configuration
    char global[512];
    git_config_find_global(global, 511);
    git_config_add_file_ondisk(cfg, global, GIT_CONFIG_LEVEL_GLOBAL, 0);

}

QString Configuration::userEmail()
{
    return getString ("user.email");
}

QString Configuration::userName()
{
    return getString ("user.name");
}

QString Configuration::getString(QString configurationOption)
{
    QString userConfiguration;
    try
    {
        const char *configuration;
        gitTest(git_config_get_string(&configuration, cfg, configurationOption.toLocal8Bit()));
        userConfiguration = QString(configuration);

        git_config_free (cfg);
        openConfigutation();
    }
    catch (GitException e)
    {
        qCritical() << __func__ << ":" << __LINE__ << " return value =" << e.returnValue();
    }
    return userConfiguration;
}


}
