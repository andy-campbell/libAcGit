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

#include "Diff.h"
#include "Diffprintcallback.h"
#include "Diffcallback.h"

namespace AcGit {

Diff::~Diff()
{

}

QStringList Diff::fileList()
{
    return filesChangedList;
}

QString Diff::fileDiff(QString file)
{
    if (filesChangedList.contains(file) == true)
    {
        return deltas[file]->patch;
    }
    else
    {
        throw fileNotInDiffException(file);
    }
}

void Diff::processDifferences()
{
    DiffCallBack diff(differences);

    filesChangedList = diff.filesChangedList();
    deltas = diff.patchDeltas();
}

QString Diff::diffStats()
{
    QString stats;

    int totalAdditions = 0;
    int totalDeletions = 0;

    foreach (QString file, filesChangedList)
    {
        diffInfo *info = deltas[file];


        totalAdditions += info->additions;
        totalDeletions += info->deletions;

        stats.append(" " + file + "\t|    " + diffString(info->additions, info->deletions) + "\n");
    }
    stats.append(" " + QString::number(filesChangedList.count()));
    stats.append(" file changed, " + QString::number(totalAdditions));
    stats.append(" insertions(+), " + QString::number(totalDeletions));
    stats.append(" deletions(-)\n");

    return stats;
}

QString Diff::diffString(int additions, int deletions)
{
    QString diffStat;

    for (int i = 0; i < additions; i++)
    {
        diffStat = diffStat + "+";
    }

    for (int i = 0; i < deletions; i++)
    {
        diffStat = diffStat + "-";
    }

    return diffStat;
}

QString Diff::printDiff()
{
    DiffPrintCallBack printer(differences);

    return printer.diffPrintPatch();
}

fileNotInDiffException::fileNotInDiffException(QString fileNotFound)
{
    this->fileNotFound = fileNotFound;
}

fileNotInDiffException::~fileNotInDiffException() throw()
{
}

QString fileNotInDiffException::exceptionMessage() const throw()
{
    QString message = "File " + fileNotFound;
    message += " was not part of the current diff";
    return message;
}

}

