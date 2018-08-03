TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

DESTDIR = $$_PRO_FILE_PWD_/bin
OBJECTS_DIR = $$_PRO_FILE_PWD_/build

QT += core
QT += qml quick widgets

SOURCES += main.cpp \
    WindowsHandler.cpp \
    WinWindow.cpp \
    UIHandler.cpp \
    QTUI.cpp \
    ThreadsManager.cpp \
    ThreadsManagerTests.cpp \
    thread_impl.cpp \
    Utils.cpp \
    OGLLogic.cpp \
    GUILogic.cpp \
    AppLogic.cpp \
    BasicControlsUIHandler.cpp \
    QtConstructs.cpp \
    BasicSceneHandler.cpp

HEADERS += \
    WindowsHandler.h \
    WinWindow.h \
    UIHandler.h \
    QTUI.h \
    AppTypes.h \
    AppManagerTypes.h \
    ThreadsManager.h \
    ThreadsManagerTests.h \
    thread_impl.h \
    Utils.h \
    OGLLogic.h \
    GUILogic.h \
    AppLogic.h \
    BasicControlsUIHandler.h \
    QtConstructs.h \
    BasicSceneHandler.h


INCLUDEPATH +=$$_PRO_FILE_PWD_/OpenGLSDK/include

#LIBS += -L$$PWD/OpenGLSDK/x86 -llibGLESv2
#LIBS += -L$$PWD/OpenGLSDK/x86 -llibEGL
#LIBS += -glew32
LIBS += -llibGLESv2
LIBS += -llibEGL
LIBS += -lgdi32
#LIBS += -lopengl32

RESOURCES += \
    uicontrol.qrc

DISTFILES += \
    AppTypes.cpp.txt
