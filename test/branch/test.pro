######################################################################
# Automatically generated by qmake (3.0) Sun May 5 16:58:26 2013
######################################################################

TEMPLATE = app
TARGET = test
CONFIG += console
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++11
QMAKE_RPATHDIR += $$PWD/../../

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -llibAcGit
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -llibAcGit
#else:unix:
LIBS += -L$$PWD/../../
LIBS += -llibAcGit
LIBS += -lgtest


# Input
SOURCES += Test-branch.cpp
