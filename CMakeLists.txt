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
        src/mainwindow.cpp                    src/mainwindow.h
        src/editor/editorinterface.cpp        src/editor/editorinterface.h
        src/editor/tabs/tabcanvas.cpp         src/editor/tabs/tabcanvas.h
        src/editor/tabs/object.cpp            src/editor/tabs/object.h
        src/editor/tabs/scene.cpp             src/editor/tabs/scene.h
        src/editor/tabs/diagram/Diagrams.cpp  src/editor/tabs/diagram/Diagrams.h)
target_link_libraries(proj
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
