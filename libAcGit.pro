#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T09:44:37
#
#-------------------------------------------------

LIBS += -lgit2 -L/usr/local/lib
QMAKE_CXXFLAGS += -std=c++11 -fPIC -Wno-missing-field-initializers -shared

TARGET = libAcGit
TEMPLATE = lib

DEFINES += LIBACGIT_LIBRARY

SOURCES += \
    Repository.cpp \
    Commits.cpp \
    Tags.cpp \
    Branches.cpp \
    Branch.cpp \
    Sha.cpp \
    Branchescallback.cpp \
    Gitexception.cpp \
    Commit.cpp \
    Tag.cpp \
    Tagscallback.cpp \
    Tree.cpp \
    TreeDiff.cpp \
    Diffcallback.cpp \
    Diff.cpp \
    Workingdirdiff.cpp \
    Diffprintcallback.cpp \
    Treeentry.cpp \
    Treeentrycallback.cpp \
    Tagger.cpp \
    Configuration.cpp \
    Blob.cpp \
    Apiexception.cpp \
    Commitgraph.cpp \
    Graph.cpp

HEADERS +=\
        libacgit_global.h \
    Repository.h \
    Commits.h \
    Tags.h \
    IBranches.h \
    Branches.h \
    Branch.h \
    Sha.h \
    Branchescallback.h \
    AcGitGlobal.h \
    Gitexception.h \
    ICommits.h \
    Commit.h \
    ITags.h \
    Tag.h \
    Tagscallback.h \
    Tree.h \
    TreeDiff.h \
    Diffcallback.h \
    Diff.h \
    Workingdirdiff.h \
    Diffprintcallback.h \
    Treeentry.h \
    Treeentrycallback.h \
    Tagger.h \
    Configuration.h \
    Blob.h \
    Apiexception.h \
    Commitgraph.h \
    Graph.h
