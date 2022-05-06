QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/editor/editorinterface.cpp \
        src/editor/tabs/TabCanvas/ClassCanvas.cpp \
        src/editor/tabs/diagram/DiagramItem.cpp \
        src/editor/tabs/diagram/EditorScene.cpp \
        src/editor/tabs/diagram/Commands.cpp \
        src/editor/tabs/diagram/Connections.cpp \
        src/editor/tabs/Itemsbuffer.cpp \
        src/editor/tabs/fillItems.cpp \
        src/editor/tabs/TabCanvas/SequenceCanvas.cpp

HEADERS += \
        src/mainwindow.h \
        src/editor/tabs/fillItems.h \
        src/editor/tabs/diagram/DiagramItem.h \
        src/editor/tabs/diagram/EditorScene.h \
        src/editor/tabs/diagram/Commands.h \
        src/editor/tabs/diagram/Connections.h \
        src/editor/editorinterface.h \
        src/editor/tabs/TabCanvas/TabCanvas.h \
        src/editor/tabs/Itemsbuffer.h

FORMS += \
    src/editor/editorinterface.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
