QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    audiocapture.cpp \
    audiodevicemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    qrcodegen.cpp \
    udpsender.cpp

HEADERS += \
    audiocapture.h \
    audiodevicemanager.h \
    mainwindow.h \
    qrcodegen.hpp \
    udpsender.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    appicon.qrc
