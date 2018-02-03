TEMPLATE = app

QT += qml quick sql
CONFIG += c++11

SOURCES += \
    SystemMgr/systemmgr.cpp \
    SystemMgr/systemmgrprivate.cpp \
    work/bluetoothprint.cpp \
    work/main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=

# Win:
WIN_OUT_ROOT = ../../bin
WIN_LIB_ROOT = ../../libs

# Android:
ANDROID_LIB_ROOT = ../../android_libs

win32{
    DESTDIR = $$WIN_OUT_ROOT
    LIBS += \
         -L$$WIN_LIB_ROOT
}

android {
    DESTDIR = $$ANDROID_LIB_ROOT
    QT += androidextras bluetooth
    LIBS += \
         -L$$ANDROID_LIB_ROOT
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/libs/LPAPI-2017-08-12.jar \
    android/src/com/NSPrinter.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += \
    SystemMgr/systemmgr.h \
    SystemMgr/systemmgrprivate.h \
    work/bluetoothprint.h
