#-------------------------------------------------
#
# Project created by QtCreator 2015-08-07T13:18:36
#
#-------------------------------------------------

QT       += widgets

TARGET = EcoCalc
TEMPLATE = lib

DEFINES += ECOCALC_LIBRARY

SOURCES += ecocalc.cpp \
    Calculate/Calculate.cpp \
    Translate/KPlane.cpp \
    Translate/KPosition.cpp \
    Translate/KSphere.cpp \
    Translate/KStraight.cpp \
    Translate/KVector.cpp \
    Translate/Translate.cpp

HEADERS += ecocalc.h\
        ecocalc_global.h \
    Calculate/Calculate.h \
    Translate/KPlane.h \
    Translate/KPosition.h \
    Translate/KSphere.h \
    Translate/KStraight.h \
    Translate/KVector.h \
    Translate/Translate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
