cmake_minimum_required(VERSION 3.17)
project(diagrams)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_PREFIX_PATH "/Users/sasha/Qt/6.2.4/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(CMAKE_CXX_FLAG DEBUG)

add_executable(proj
        src/main.cpp
        src/MainWIndow/mainwindow.cpp
        src/editor/EditorInterface/editorinterface.cpp
        src/editor/PropertiesDialog/propertiesdialog.cpp
        src/editor/Tabs/TabCanvas/ClassCanvas.cpp
        src/editor/Tabs/TabCanvas/SequenceConnectionDialog/sequenceconnectiondialog.cpp
        src/editor/Tabs/DiagramItem/DiagramItem.cpp
        src/editor/Tabs/EditorScene/EditorScene.cpp
        src/editor/Tabs/Commands/Commands.cpp
        src/editor/Tabs/Connections/ClassConnectinos.cpp
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.cpp
        src/editor/Tabs/FillItems/ObjectParams.cpp
        src/editor/Tabs/TabCanvas/SequenceCanvas.cpp
        src/editor/Backend/ClassDiagram.cpp
        src/editor/Backend/Parse.cpp
        src/editor/Backend/SeqDiagram.cpp
        src/editor/Backend/json.hpp
        src/editor/Tabs/Connections/SequenceConnections.cpp
        src/editor/Tabs/DiagramItem/ClassDiagramItem.cpp
        src/editor/Tabs/DiagramItem/SequenceDiagramItem.cpp
        src/editor/Tabs/DiagramItem/SequenceDiagramLifeLine/SequenceDiagramLifeLine.cpp

        src/MainWIndow/mainwindow.h
        src/editor/PropertiesDialog/propertiesdialog.h
        src/editor/Tabs/FillItems/ObjectParams.h
        src/editor/Tabs/DiagramItem/DiagramItem.h
        src/editor/Tabs/EditorScene/EditorScene.h
        src/editor/Tabs/Commands/Commands.h
        src/editor/Tabs/Connections/Connections.h
        src/editor/EditorInterface/editorinterface.h
        src/editor/Tabs/TabCanvas/TabCanvas.h
        src/editor/Tabs/TabCanvas/SequenceConnectionDialog/sequenceconnectiondialog.h
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.h
        src/editor/Backend/ClassDiagram.h
        src/editor/Backend/Parse.h
        src/editor/Backend/SeqDiagram.h
        src/editor/Tabs/DiagramItem/SequenceDiagramLifeLine/SequenceDiagramLifeLine.h
        )

target_link_libraries(proj
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
