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


#ifndef COMMIT_H
#define COMMIT_H

#include <QString>
#include <QDateTime>

#include "Sha.h"
#include "Repository.h"
#include "Gitexception.h"
#include "Tree.h"
#include "Graph.h"

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

            Tree *tree() const;

            git_commit* rawCommit();

            void addGraph(Graph* graph);
            Graph* getGraph();

            int parentCount();
            Sha* parentSha(int index);
    private:
            void buildTree();
            void populateParentSha();

            Tree* commitTree;
            git_commit* commit;
            Graph* graph;
            Repository* repo;
            void commitLookupExecption(GitException e);
            QList<Sha *> parentShas;
    };

}
#endif // COMMIT_H
