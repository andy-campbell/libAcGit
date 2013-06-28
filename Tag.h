#ifndef TAG_H
#define TAG_H

#include <QString>

#include "Sha.h"
#include "Tagger.h"


namespace AcGit
{


    class Tag : public Sha
    {
        public:
            Tag(QString name, Sha *sha, git_tag *tag);

            QString name();
            QString message();
            QString refName();

            Tagger *getTagger();
    private:
            QString tagName;
            git_tag *tag;
    };

}
#endif // TAG_H
