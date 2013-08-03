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

#include "Workingdirdiff.h"
#include "Commit.h"

namespace AcGit
{

WorkingDirDiff::WorkingDirDiff(Tree *headCommitTree)
{
    // get the diff
    const git_diff_options options = GIT_DIFF_OPTIONS_INIT;
    Repository *repo = headCommitTree->getRepository();


    try
    {
        gitTest(git_diff_tree_to_workdir (&differences, repo->getInternalRepo(),
                                          headCommitTree->internalTree() , &options));
    }
    catch(GitException e)
    {
        workingTreeDiffException(e);
    }

    processDifferences();
}

WorkingDirDiff::~WorkingDirDiff()
{

}

void WorkingDirDiff::workingTreeDiffException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}

}
