# Reference: https://doc.qt.io/qt-6/cmake-manual.html

cmake_minimum_required(VERSION 3.30)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
project(pi VERSION 1.0)

set(SOURCE_FILES
    src/main.cpp
    src/AbstractViewer.cpp
    src/HistogramViewer.cpp
    src/ImageGrid.cpp
    src/ImageGridCell.cpp
    src/ImageViewer.cpp
    src/Tools.cpp
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt_standard_project_setup()

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)