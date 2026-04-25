cmake_minimum_required(VERSION 3.16)
project(CollegeBusManagementSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if (Qt6_FOUND)
    set(QT_WIDGETS_LIB Qt6::Widgets)
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_WIDGETS_LIB Qt5::Widgets)
endif()

add_executable(CollegeBusManagementSystem
    main.cpp
    mainwindow.cpp
    student.cpp
)

target_link_libraries(CollegeBusManagementSystem PRIVATE ${QT_WIDGETS_LIB})
