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
#include <QStringList>
#include <QRegExp>

#include "Commit.h"

namespace AcGit
{

Commit::Commit(Repository *repo, Sha *commitSha)
    : Sha (commitSha)
{
    this->repo = repo;
    try
    {
        gitTest(git_commit_lookup(&commit, repo->getInternalRepo(), sha));
    }
    catch (GitException e)
    {
        commitLookupExecption (e);
    }
    this->graph = nullptr;
    this->commitTree = nullptr;

    populateParentSha();
}

void Commit::populateParentSha()
{
    for(int parent = 0; parent < parentCount(); parent++)
    {
        const git_oid *parentSha = git_commit_parent_id(commit, parent);
        parentShas.insert(parent, new Sha(parentSha));
    }
}

void Commit::buildTree()
{
    git_tree *gitTree;
    gitTest(git_commit_tree (&gitTree, commit));

    commitTree = new Tree (repo, gitTree);
}


Commit::~Commit()
{

}

void Commit::commitLookupExecption(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}


QString Commit::log() const
{
    QRegExp regex("(\\r|\\n)");
    QString log = QString(git_commit_message(commit));

    log = log.remove(0, log.indexOf(regex) + 1);
    return log;
}


QString Commit::shortLog()
{
    QRegExp regex("(\\r|\\n)");
    QString log = QString(git_commit_message(commit));

    QString shortLog = log.left(80).split(regex).first();
    return shortLog;
}

QString Commit::author()
{
    const git_signature* signature = git_commit_author(commit);

    return QString(signature->name);
}

QString Commit::authorsEmail()
{
    const git_signature* signature = git_commit_author(commit);

    return QString(signature->email);
}

QString Commit::committer()
{
    const git_signature* signature = git_commit_committer (commit);

    return QString(signature->name);
}

QString Commit::committerEmail()
{
    const git_signature* signature = git_commit_committer (commit);

    return QString(signature->email);
}

Tree *Commit::tree()
{
    if(commitTree == nullptr)
    {
        buildTree();
    }


    return commitTree;
}

git_commit *Commit::rawCommit()
{
    return commit;
}

void Commit::addGraph(Graph *graph)
{
    if(this->graph)
        delete this->graph;

    this->graph = graph;
}

Graph *Commit::getGraph()
{
    return graph;
}


QDateTime Commit::dateCreated()
{
    QDateTime dateTime;
    git_time_t time = git_commit_time(commit);
    dateTime.setTime_t(time);
    return dateTime;
}

int Commit::parentCount()
{
    int numParents = git_commit_parentcount(commit);
    return numParents;
}

Sha *Commit::parentSha(int index)
{
    Sha *parent;
    if (index <= parentCount())
    {
        parent = parentShas.at(index);
    }

    return parent;
}



}
