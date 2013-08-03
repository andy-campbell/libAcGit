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
