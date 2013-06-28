#ifndef DIFFPRINTCALLBACK_H
#define DIFFPRINTCALLBACK_H

#include <QString>
#include <git2.h>

namespace AcGit
{

    class DiffPrintCallBack
    {
        public:
            DiffPrintCallBack(git_diff_list *differences);

            QString diffPrintPatch();

            void addToPAtch(const char *line);
    private:
            QString patch;
    };
}
#endif // DIFFPRINTCALLBACK_H
