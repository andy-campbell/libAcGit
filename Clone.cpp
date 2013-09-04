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

#include "Clone.h"
#include <QDebug>

namespace AcGit
{
extern "C" int fetch_progress(const git_transfer_progress *stats, void *payload)
{
    int retVal = 0;
    if (reinterpret_cast<Clone*> (payload)->updateFetchProgress(stats) == true)\
    {
        retVal = -1;
    }
    return retVal;

}
extern "C" void checkout_progress(const char *path, size_t cur, size_t tot, void *payload)
{
    reinterpret_cast<Clone*> (payload)->updateCheckoutProgress(path, cur, tot);
}

extern "C" int cred_acquire(git_cred **out,
                const char * url,
                const char * username_from_url,
                unsigned int allowed_types,
                void * payload)
{
    Credintials creds = reinterpret_cast<Clone*> (payload)->aquireCredentials();
    QString password = creds.getPassword();
    QString username = creds.getUsername();

    return git_cred_userpass_plaintext_new(out, username.toLocal8Bit(), password.toLocal8Bit());
}



Clone::Clone(QString url, QString path,
             std::function<void(Clone*)> checkOutProgressCb,
             std::function<bool(Clone*)> fetchProgressCb,
             std::function<Credintials(void)> credentialsCb
             )
{
    this->checkOutProgressCb = checkOutProgressCb;
    this->fetchProgressCb = fetchProgressCb;
    this->credentialsCb = credentialsCb;
    cancel = false;
    completedSteps = -1;
    totalSteps = -1;

    git_repository *clonedRepo = NULL;
    git_clone_options cloneOpts = GIT_CLONE_OPTIONS_INIT;
    git_checkout_opts checkoutOpts = GIT_CHECKOUT_OPTS_INIT;

    // Set up options
    checkoutOpts.checkout_strategy = GIT_CHECKOUT_SAFE_CREATE;
    checkoutOpts.progress_cb = checkout_progress;
    checkoutOpts.progress_payload = this;
    cloneOpts.checkout_opts = checkoutOpts;
    cloneOpts.fetch_progress_cb = &fetch_progress;
    cloneOpts.fetch_progress_payload = this;
    cloneOpts.cred_acquire_cb = cred_acquire;

    // Do the clone
    gitTest(git_clone(&clonedRepo, url.toLocal8Bit(), path.toLocal8Bit(), &cloneOpts));

    openRepository(clonedRepo);

}

void Clone::openRepository(git_repository* internalRepo)
{
    repo = new Repository(internalRepo);
}

void Clone::updateCheckoutProgress(const char *newPath, size_t newCompletedSteps, size_t newTotalSteps)
{
    path = QString(newPath);
    totalSteps = newTotalSteps;
    completedSteps = newCompletedSteps;
    checkOutProgressCb(this);
}

bool Clone::updateFetchProgress(const git_transfer_progress *stats)
{
    fetchProgress = *stats;
    fetchProgressCb(this);

    return cancel;
}

void Clone::cancelClone()
{
    cancel = true;
}

int Clone::networkPercentage()
{
    return (100 * fetchProgress.received_objects) / fetchProgress.total_objects;
}

int Clone::indexPercentage()
{
    return (100 * fetchProgress.indexed_objects) / fetchProgress.total_objects;
}

int Clone::checkoutPercentage()
{
    return (100 * completedSteps) / totalSteps;
}

int Clone::kbytesRecieved()
{
    return fetchProgress.received_bytes / 1024;
}

Credintials Clone::aquireCredentials()
{
    Credintials creds = credentialsCb();
    return creds;
}

Repository *Clone::getRepo()
{
    return repo;
}
}
