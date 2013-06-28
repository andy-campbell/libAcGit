#ifndef DIFF_H
#define DIFF_H

#include <QStringList>
#include <QString>
#include <QMap>
#include <git2.h>
#include <exception>


namespace AcGit {


    class Diff
    {
        public:
            virtual ~Diff();

            typedef struct
            {
                QString patch;
                int additions;
                int deletions;
            } diffInfo;

            virtual QStringList fileList();
            virtual QString fileDiff(QString file);
            virtual QString diffStats();
            QString printDiff();

        protected:

            git_diff_list* differences;

            QMap<QString, diffInfo*> deltas;
            QStringList filesChangedList;

            void processDifferences();

        private:
            QString diffString(int additions, int deletions);


    };

    class fileNotInDiffException : public std::exception
    {
        public:
            fileNotInDiffException(QString fileNotFound);

            ~fileNotInDiffException() throw();

            QString exceptionMessage() const throw();

            QString fileNotFound;

    };

}
#endif // DIFF_H
