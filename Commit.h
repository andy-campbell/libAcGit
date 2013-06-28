#ifndef COMMIT_H
#define COMMIT_H

#include <QString>
#include <QDateTime>

#include "Sha.h"
#include "Repository.h"
#include "Gitexception.h"
#include "Tree.h"

namespace AcGit
{
    class Repository;
    class Tree;

    class Commit : public Sha
    {
        public:
            Commit(Repository *repo, Sha *commitSha);
            ~Commit();


            QString shortLog();
            QString log() const;

            QString author();
            QDateTime dateCreated();
            QString authorsEmail();
            QString committer();
            QString committerEmail();

            Tree* tree() const;

            git_commit* rawCommit();

    private:
            git_commit* commit;
            Repository* repo;
            void commitLookupExecption(GitException e);
    };

}
#endif // COMMIT_H
