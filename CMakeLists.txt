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
        src/mainwindow.cpp
        src/editor/editorinterface.cpp
        src/editor/tabs/TabCanvas/ClassCanvas.cpp
        src/editor/tabs/diagram/DiagramItem.cpp
        src/editor/tabs/diagram/EditorScene.cpp
        src/editor/tabs/diagram/Commands.cpp
        src/editor/tabs/diagram/Connections.cpp
        src/editor/tabs/Itemsbuffer.cpp
        src/editor/tabs/fillItems.cpp
        src/editor/tabs/TabCanvas/SequenceCanvas.cpp

        src/mainwindow.h
        src/editor/tabs/fillItems.h
        src/editor/tabs/diagram/DiagramItem.h
        src/editor/tabs/diagram/EditorScene.h
        src/editor/tabs/diagram/Commands.h
        src/editor/tabs/diagram/Connections.h
        src/editor/editorinterface.h
        src/editor/tabs/TabCanvas/TabCanvas.h
        src/editor/tabs/Itemsbuffer.h
        )
target_link_libraries(proj
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
