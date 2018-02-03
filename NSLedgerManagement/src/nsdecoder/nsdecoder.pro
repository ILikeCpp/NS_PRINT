#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T11:20:59
#
#-------------------------------------------------

QT       += quick multimedia

QT       -= gui

TARGET = nsdecoder
TEMPLATE = lib

DEFINES += NSDECODER_LIBRARY

INCLUDEPATH += ../QZXing

SOURCES += nsdecoder.cpp \
    nsdecoder_p.cpp \
    nsvideofilter.cpp

HEADERS += nsdecoder.h\
        nsdecoder_global.h \
    nsdecoder_p.h \
    nsvideofilter.h \
    nsglobal.h

# Win:
WIN_OUT_ROOT = ../../bin
WIN_LIB_ROOT = ../../libs

# Android:
ANDROID_LIB_ROOT = ../../android_libs


contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/$$ANDROID_LIB_ROOT/libQZXing.so

    LIBS += \
        -L$$ANDROID_LIB_ROOT \
        -lQZXing
}
contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/$$ANDROID_LIB_ROOT/libQZXing.so

    LIBS += \
        -L$$ANDROID_LIB_ROOT \
        -lQZXing
}
contains(ANDROID_TARGET_ARCH,armeabi) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/$$ANDROID_LIB_ROOT/libQZXing.so

    LIBS += \
        -L$$ANDROID_LIB_ROOT \
        -lQZXing
}

win32 {
    DESTDIR = $$WIN_LIB_ROOT
    LIBS += \
        -L$$WIN_LIB_ROOT \
        -lQZXing2
}

android {
    DESTDIR = $$ANDROID_LIB_ROOT
    QT += androidextras
}
