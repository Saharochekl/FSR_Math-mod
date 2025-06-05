QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    atmosphere.cpp \
    atmosphereconst.cpp \
    dynamic_atmosphere.cpp \
    exponentialatmosphere.cpp \
    main.cpp \
    mainwindow.cpp \
    temperatureprofile.cpp \
    msise90_sub.c


HEADERS += \
    atmosphere.h \
    atmosphereconst.h \
    dynamic_atmosphere.h \
    exponentialatmosphere.h \
    mainwindow.h \
    msise90_sub.h \
    temperatureprofile.h

FORMS += \
    mainwindow.ui

unix: {
    INCLUDEPATH += /usr/local/include
    LIBS        += -L/usr/local/lib -lf2c -lm
    # Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
}

win32-g++ {                       # MinGW / MSYS2
    INCLUDEPATH += $$PWD/thirdparty/f2c/include
    LIBS        += -L$$PWD/thirdparty/f2c/lib -lf2c
}

win32-msvc {                      # MSVC toolchain
    INCLUDEPATH += $$PWD\\thirdparty\\f2c\\include
    LIBS        += $$PWD\\thirdparty\\f2c\\lib\\libf2c.lib
}

