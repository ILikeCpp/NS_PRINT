#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T15:53:33
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = nsfileaccess
TEMPLATE = lib

DEFINES += NSFILEACCESS_LIBRARY

SOURCES += \
    nsfileaccess.cpp

HEADERS +=\
    nsfileaccess.h

# Win:
WIN_LIB_ROOT = ../../libs

# Android:
ANDROID_LIB_ROOT = ../../android_libs


win32 {
    DESTDIR = $$WIN_LIB_ROOT
}

android {
    DESTDIR = $$ANDROID_LIB_ROOT
}
