QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/main.cpp  \
        src/MainWIndow/mainwindow.cpp \
        src/editor/EditorInterface/editorinterface.cpp \
        src/editor/PropertiesDialog/propertiesdialog.cpp \
        src/editor/Tabs/TabCanvas/ClassCanvas.cpp \
        src/editor/Tabs/DiagramItem/DiagramItem.cpp \
        src/editor/Tabs/DiagramItem/EditorScene/EditorScene.cpp \
        src/editor/Tabs/DiagramItem/Commands/Commands.cpp \
        src/editor/Tabs/DiagramItem/Connections/Connections.cpp \
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.cpp \
        src/editor/Tabs/DiagramItem/FillItems/FillItems.cpp \
        src/editor/Tabs/TabCanvas/SequenceCanvas.cpp

HEADERS += \
        src/MainWIndow/mainwindow.h \
        src/editor/PropertiesDialog/propertiesdialog.h \
        src/editor/Tabs/DiagramItem/FillItems/FillItems.h \
        src/editor/Tabs/DiagramItem/DiagramItem.h \
        src/editor/Tabs/DiagramItem/EditorScene/EditorScene.h \
        src/editor/Tabs/DiagramItem/Commands/Commands.h \
        src/editor/Tabs/DiagramItem/Connections/Connections.h \
        src/editor/EditorInterface/editorinterface.h \
        src/editor/Tabs/TabCanvas/TabCanvas.h \
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.h

FORMS += \
    src/editor/EditorInterface/editorinterface.ui \
    src/editor/PropertiesDialog/propertiesdialog.ui \
    src/MainWindow/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
