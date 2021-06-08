QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    checkSig.h \
    description.h \
    mainwindow.h \
    menu.h \
    reg.h \
    sig.h \
    task.h

FORMS += \
    mainwindow.ui

LIBS += \
    -lkernel32 \
    -ladvapi32 \
    -lversion \
    -lwintrust \
    -lcrypt32 \
    -lole32 \
    -loleaut32 \
    -ltaskschd \
    -lcomsupp \
    -lShlwapi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
