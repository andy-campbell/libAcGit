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

