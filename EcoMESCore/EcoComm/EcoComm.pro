#-------------------------------------------------
#
# Project created by QtCreator 2015-07-28T17:05:04
#
#-------------------------------------------------

QT += widgets serialport serialport

TARGET = EcoComm
TEMPLATE = lib

DEFINES += ECOCOMM_LIBRARY

SOURCES += ecocomm.cpp \
    ecoserialmanager.cpp \
    eutility.cpp

HEADERS +=\
        ecocomm_global.h \
    ecocomm.h \
    ecoserialmanager.h \
    eutility.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
