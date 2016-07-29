include(TestWindowsService.pri)

QT += core
QT -= gui


TARGET = TestWindowsService
TEMPLATE = app

DESTDIR = $$PWD/build/$$PROJ_SPEC

DEFINES += _UNICODE

CONFIG += c++11 \
          console
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/qt-service-lib/debug \
            -lQtSolutions_Service-headd
} else {
    LIBS += -L$$PWD/qt-service-lib/release \
            -lQtSolutions_Service-head
}

LIBS += -ladvapi32 \
        -lkernel32 \
        -lWtsapi32 \
        -lUser32 \
        -lUserEnv

INCLUDEPATH += $$PWD/qt-service-lib/include

SOURCES += main.cpp \
    my_windows_service.cpp \
    message_handler.cpp \
    windowassist.cpp

HEADERS += \
    my_windows_service.h \
    qt-service-lib/include/qtservice.h \
    message_handler.h \
    windowassist.h

DISTFILES += \
    TestWindowsService.pri

copyDependency()
