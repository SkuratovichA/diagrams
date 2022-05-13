QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/main.cpp \
        src/editor/Backend/Parse.cpp \
        src/MainWIndow/mainwindow.cpp \
        src/editor/Backend/SeqDiagram.cpp \
        src/editor/Backend/ClassDiagram.cpp \
        src/editor/Tabs/Commands/Commands.cpp \
        src/editor/Tabs/TabCanvas/ClassCanvas.cpp \
        src/editor/Tabs/FillItems/ObjectParams.cpp \
        src/editor/Tabs/DiagramItem/DiagramItem.cpp \
        src/editor/Tabs/EditorScene/EditorScene.cpp \
        src/editor/Tabs/TabCanvas/SequenceCanvas.cpp \
        src/editor/EditorInterface/editorinterface.cpp \
        src/editor/Tabs/Connections/ClassConnections.cpp \
        src/editor/PropertiesDialog/propertiesdialog.cpp \
        src/editor/Tabs/DiagramItem/ClassDiagramItem.cpp \
        src/editor/Tabs/DiagramItem/SequenceDiagramItem.cpp \
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.cpp \
        src/editor/Tabs/Connections/SequenceConnectionItem.cpp \
        src/editor/Tabs/TabCanvas/SequenceConnectionDialog/sequenceconnectiondialog.cpp \
        src/editor/Tabs/DiagramItem/SequenceDiagramLifeLine/SequenceDiagramLifeLine.cpp

HEADERS += \
        src/editor/Backend/Parse.h \
        src/editor/Backend/json.hpp \
        src/MainWIndow/mainwindow.h \
        src/editor/Backend/SeqDiagram.h \
        src/editor/Backend/ClassDiagram.h \
        src/editor/Tabs/Commands/Commands.h \
        src/editor/Tabs/TabCanvas/TabCanvas.h \
        src/editor/Tabs/FillItems/ObjectParams.h \
        src/editor/Tabs/DiagramItem/DiagramItem.h \
        src/editor/Tabs/EditorScene/EditorScene.h \
        src/editor/Tabs/Connections/Connections.h \
        src/editor/EditorInterface/editorinterface.h \
        src/editor/PropertiesDialog/propertiesdialog.h \
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.h \
        src/editor/Tabs/TabCanvas/SequenceConnectionDialog/sequenceconnectiondialog.h \
        src/editor/Tabs/DiagramItem/SequenceDiagramLifeLine/SequenceDiagramLifeLine.h

FORMS += \
    src/MainWIndow/mainwindow.ui \
    src/editor/EditorInterface/editorinterface.ui \
    src/editor/PropertiesDialog/propertiesdialog.ui \
    src/editor/Tabs/TabCanvas/SequenceConnectionDialog/sequenceconnectiondialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
