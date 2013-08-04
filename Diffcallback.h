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


#ifndef DIFFCALLBACK_H
#define DIFFCALLBACK_H

#include <git2.h>
#include <QMap>
#include <QStringList>

#include "Gitexception.h"
#include "TreeDiff.h"
#include "Diff.h"

namespace AcGit
{
    class Tree;
    class Diff;

    class DiffCallBack
    {
        public:
            DiffCallBack(git_diff_list* differences);
            void addPatchLines(const git_diff_delta *delta, const char *line, char usage, int lineLen);
            void addPatchHunks(const git_diff_delta *delta, const char *header, int headerLen);
            void addFileChanged(const git_diff_delta *delta);

            QStringList filesChangedList();

            QMap<QString, Diff::diffInfo*> patchDeltas();


        private:

            QMap<QString, Diff::diffInfo*> deltas;
            QStringList fileList;

            void diffForEachException(GitException e);

    };


}
#endif // DIFFCALLBACK_H
