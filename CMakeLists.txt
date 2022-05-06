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

add_executable(proj
        src/main.cpp
        src/MainWIndow/mainwindow.cpp
        src/editor/EditorInterface/editorinterface.cpp
        src/editor/PropertiesDialog/propertiesdialog.cpp
        src/editor/Tabs/TabCanvas/ClassCanvas.cpp
        src/editor/Tabs/DiagramItem/DiagramItem.cpp
        src/editor/Tabs/DiagramItem/EditorScene/EditorScene.cpp
        src/editor/Tabs/DiagramItem/Commands/Commands.cpp
        src/editor/Tabs/DiagramItem/Connections/Connections.cpp
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.cpp
        src/editor/Tabs/DiagramItem/FillItems/FillItems.cpp
        src/editor/Tabs/TabCanvas/SequenceCanvas.cpp
        src/editor/backend/class_diagram.cpp
        src/editor/backend/parse.cpp
        src/editor/backend/seq_diagram.cpp

        src/MainWIndow/mainwindow.h
        src/editor/PropertiesDialog/propertiesdialog.h
        src/editor/Tabs/DiagramItem/FillItems/FillItems.h
        src/editor/Tabs/DiagramItem/DiagramItem.h
        src/editor/Tabs/DiagramItem/EditorScene/EditorScene.h
        src/editor/Tabs/DiagramItem/Commands/Commands.h
        src/editor/Tabs/DiagramItem/Connections/Connections.h
        src/editor/EditorInterface/editorinterface.h
        src/editor/Tabs/TabCanvas/TabCanvas.h
        src/editor/Tabs/TabCanvas/ItemsBuffer/Itemsbuffer.h
        src/editor/backend/class_diagram.h
        src/editor/backend/parse.h
        src/editor/backend/seq_diagram.h
        src/editor/backend/json.hpp
        )
target_link_libraries(proj
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
