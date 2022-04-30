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
    src/editor/tabs/tabcanvas.cpp \
    src/editor/tabs/object.cpp \
    src/editor/tabs/scene.cpp \
    src/editor/tabs/diagram/Diagrams.cpp \

HEADERS += \
    src/mainwindow.h \
    src/editor/editorinterface.h \
    src/editor/tabs/tabcanvas.h \
    src/editor/tabs/object.h \
    src/editor/tabs/scene.h \
    src/editor/tabs/diagram/Diagrams.h) \

FORMS += \
    src/editor/editorinterface.ui \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
