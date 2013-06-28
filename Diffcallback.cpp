#include <QDebug>

#include "Diffcallback.h"


namespace AcGit
{

extern "C" int fileDiffCallBack(const git_diff_delta *delta,float progress,void *payload)
{
    reinterpret_cast<DiffCallBack*> (payload)->addFileChanged(delta);
    // we want to continue looping so return 0
    return 0;
}


extern "C" int addPatchHunksCallBack( const git_diff_delta *delta, const git_diff_range *range,
                                        const char *header, size_t header_len, void *payload)
{
    reinterpret_cast<DiffCallBack*> (payload)->addPatchHunks(delta, header, header_len);
    // we want to continue looping so return 0
    return 0;
}


extern "C" int addPatchLinesCallBack(const git_diff_delta *delta, const git_diff_range *range,
                                        char usage, const char *line, size_t line_len, void *payload)
{
    reinterpret_cast<DiffCallBack*> (payload)->addPatchLines(delta, line, usage, line_len);
    // we want to continue looping so return 0
    return 0;
}


DiffCallBack::DiffCallBack(git_diff_list *differences)
{
    try
    {
        gitTest(git_diff_foreach (differences, fileDiffCallBack, addPatchHunksCallBack, addPatchLinesCallBack, this));
    }
    catch (GitException e)
    {
        diffForEachException(e);
    }

}

void DiffCallBack::diffForEachException(GitException e)
{
    qCritical() << __func__ << ":" << __LINE__<< " " << e.what();
}

void DiffCallBack::addPatchLines(const git_diff_delta *delta, const char *line, char usage, int lineLen)
{

    QString fileName(delta->new_file.path);
    QString currentLine(line);
    QString realLine = currentLine.left(lineLen);

    Diff::diffInfo info = *deltas[QString(delta->new_file.path)];
    QString currentFilePatch = info.patch;

    currentFilePatch.append(usage);
    currentFilePatch.append(realLine);

    if (usage == '+')
    {
        info.additions = info.additions + 1;
    }

    if (usage == '-')
    {
        info.deletions = info.deletions + 1;
    }

    deltas[fileName]->patch = currentFilePatch;
    deltas[fileName]->additions = info.additions;
    deltas[fileName]->deletions = info.deletions;
}


void DiffCallBack::addPatchHunks(const git_diff_delta *delta, const char *header, int headerLen)
{
    QString fileName(delta->new_file.path);
    QString currentFilePatch = deltas[QString(delta->new_file.path)]->patch;
    QString currentHeader(header);
    QString realHeader = currentHeader.left(headerLen);

    currentFilePatch.append(realHeader);

    deltas[fileName]->patch = currentFilePatch;
}

void DiffCallBack::addFileChanged(const git_diff_delta *delta)
{
    fileList.push_back(QString::fromLocal8Bit(delta->new_file.path));
    Diff::diffInfo *info = new Diff::diffInfo;
    info->patch = QString();
    info->additions = 0;
    info->deletions = 0;

    deltas.insert(QString(delta->new_file.path), info);
}

QStringList DiffCallBack::filesChangedList()
{
    return fileList;
}

QMap<QString, Diff::diffInfo*> DiffCallBack::patchDeltas()
{
    return deltas;
}


}
