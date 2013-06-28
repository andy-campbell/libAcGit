#include "Diffprintcallback.h"

namespace AcGit
{

extern "C" int printCB(  const git_diff_delta *delta, const git_diff_range *range,
                         char usage, const char *line, size_t line_len, void *payload)
{
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

