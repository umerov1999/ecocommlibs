#-------------------------------------------------
#
# Project created by QtCreator 2015-08-10T09:54:19
#
#-------------------------------------------------

QT       += widgets

TARGET = EcoMesDataManager
TEMPLATE = lib

DEFINES += ECOMESDATAMANAGER_LIBRARY

SOURCES += ecomesdatamanager.cpp \
    CoordManager.cpp \
    CSV/CSV.cpp \
    Translate/KPlane.cpp \
    Translate/KPosition.cpp \
    Translate/KSphere.cpp \
    Translate/KStraight.cpp \
    Translate/KVector.cpp \
    Translate/Translate.cpp \
    EmsVersionManager.cpp \
    Calculate/Calculate.cpp \
    StringConverter.cpp \
    DesVersionManager.cpp \
    EUtility/eutility.cpp

HEADERS += ecomesdatamanager.h\
        ecomesdatamanager_global.h \
    CoordManager.h \
    CSV/CSV.h \
    Translate/KPlane.h \
    Translate/KPosition.h \
    Translate/KSphere.h \
    Translate/KStraight.h \
    Translate/KVector.h \
    Translate/Translate.h \
    EmsVersionManager.h \
    Calculate/Calculate.h \
    StringConverter.h \
    DesVersionManager.h \
    EUtility/eutility.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
