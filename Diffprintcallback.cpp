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

#include "Diffprintcallback.h"

namespace AcGit
{

extern "C" int printCB(  const git_diff_delta *delta, const git_diff_range *range,
                         char usage, const char *line, size_t line_len, void *payload)
{
    Q_UNUSED(delta);
    Q_UNUSED(range);
    Q_UNUSED(usage);
    Q_UNUSED(line);
    Q_UNUSED(line_len);

    reinterpret_cast<DiffPrintCallBack*> (payload)->addToPAtch(line);

    return 0;
}



DiffPrintCallBack::DiffPrintCallBack(git_diff_list *differences)
{
    git_diff_print_patch(differences, printCB, this);
}

void DiffPrintCallBack::addToPAtch(const char *line)
{
    patch += QString(line);
}

QString DiffPrintCallBack::diffPrintPatch()
{
    return patch;
}

}

