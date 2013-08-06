/*

Copyright (c) 2013 Andrew Campbell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

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
    if (git_config_find_global(global, 511) == 0)
    {
        git_config_add_file_ondisk(cfg, global, GIT_CONFIG_LEVEL_GLOBAL, 0);
    }

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
