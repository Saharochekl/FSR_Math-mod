QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0




CONFIG(release, debug|release): LIBS += -L$$PWD/../bin -ltleLib
CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin -ltleLibd


INCLUDEPATH += $$PWD/../tleLib \
               $$PWD/../tleLib/core \
               $$PWD/../tleLib/orbit

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    gravitymodel.cpp \
    model.cpp \
    odemodel.cpp \
    reactionmodel.cpp \
    sqpmodel.cpp \
    atmosphere.cpp \
    exp_atmosphere.cpp

HEADERS += \
    mainwindow.h \
    qcustomplot.h \
    gravitymodel.h \
    model.h \
    odemodel.h \
    reactionmodel.h \
    sqpmodel.h \
    atmosphere.h \
    exp_atmosphere.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
