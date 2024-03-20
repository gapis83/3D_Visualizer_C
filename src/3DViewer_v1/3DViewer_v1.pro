QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../parser/s21_viewer.c \
    QtGifImage-master/src/3rdParty/giflib/dgif_lib.c \
    QtGifImage-master/src/3rdParty/giflib/egif_lib.c \
    QtGifImage-master/src/3rdParty/giflib/gif_err.c \
    QtGifImage-master/src/3rdParty/giflib/gif_font.c \
    QtGifImage-master/src/3rdParty/giflib/gif_hash.c \
    QtGifImage-master/src/3rdParty/giflib/gifalloc.c \
    QtGifImage-master/src/3rdParty/giflib/quantize.c \
    QtGifImage-master/src/gifimage/qgifimage.cpp \
    glviewer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../parser/s21_viewer.h \
    QtGifImage-master/src/3rdParty/giflib/gif_hash.h \
    QtGifImage-master/src/3rdParty/giflib/gif_lib.h \
    QtGifImage-master/src/3rdParty/giflib/gif_lib_private.h \
    QtGifImage-master/src/gifimage/qgifglobal.h \
    QtGifImage-master/src/gifimage/qgifimage.h \
    QtGifImage-master/src/gifimage/qgifimage_p.h \
    glviewer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
