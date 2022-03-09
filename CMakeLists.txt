cmake_minimum_required(VERSION 3.20)
project(diagrams)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(proj main.cpp mainwindow.cpp editorinterface.cpp)
target_link_libraries(proj
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

