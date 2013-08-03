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

#ifndef SHA_H
#define SHA_H

#include <QByteArray>
#include <QString>
#include <git2.h>

#include "libacgit_global.h"

namespace AcGit
{

    class LIBACGITSHA_EXPORT Sha
    {
    public:
        Sha(const git_oid *shaPtr = nullptr);
        Sha(const Sha *other);
        Sha(QString shaString);

        virtual ~Sha();

        QByteArray toString();

        bool operator==(const Sha *sha);
        bool operator!=(const Sha *sha);

        // for internal use only
        const git_oid *raw();

    protected:
        const git_oid *sha;
    };
}
#endif // SHA_H
