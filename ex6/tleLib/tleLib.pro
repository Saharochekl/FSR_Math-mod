QT       += core gui

TARGET = $$qtLibraryTarget(tleLib)
TEMPLATE = lib

#CONFIG += debug

CONFIG += build_all shared

CONFIG(release, debug|release): TARGET = tleLib
CONFIG(debug, debug|release): TARGET = tleLibd

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11


DEFINES += TLELIB_LIBRARY

SOURCES += tleLib.cpp \
    core/cEci.cpp \
    core/cJulian.cpp \
    core/coord.cpp \
    core/cSite.cpp \
    core/cTLE.cpp \
    core/cVector.cpp \
    core/globals.cpp \
    core/stdafx.cpp \
    orbit/cNoradBase.cpp \
    orbit/cNoradSDP4.cpp \
    orbit/cNoradSGP4.cpp \
    orbit/cOrbit.cpp \
    orbit/cSatellite.cpp \
    orbit/stdafx.cpp \
    tleparser.cpp
HEADERS += ballisticlib.h\
    tleLib_global.h\
    core/cEci.h \
    core/cJulian.h \
    core/coord.h \
    core/coreLib.h \
    core/cSite.h \
    core/cTle.h \
    core/cVector.h \
    core/exceptions.h \
    core/globals.h \
    core/stdafx.h \
    orbit/cNoradBase.h \
    orbit/cNoradSDP4.h \
    orbit/cNoradSGP4.h \
    orbit/cOrbit.h \
    orbit/cSatellite.h \
    orbit/orbitLib.h \
    orbit/stdafx.h \ \
    tleparser.h

INCLUDEPATH += $$PWD/core $$PWD/orbit $$PWD/Interpolation \

win32||unix {
  target.path = $$PWD/../bin
  INSTALLS += target
}
