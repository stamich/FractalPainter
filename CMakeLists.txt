cmake_minimum_required(VERSION 3.7)
project(Fractals)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_PREFIX_PATH "/usr/lib64/qt5/bin/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Network REQUIRED )
find_package( Qt5Sql REQUIRED )
find_package( Qt5Xml REQUIRED )
find_package( Qt5PrintSupport REQUIRED )
find_package( Qt5OpenGL REQUIRED)

set(SOURCE_FILES main.cpp MainFractalWidget.cpp MainFractalWidget.h)
add_executable(Fractals ${SOURCE_FILES})
target_link_libraries(Fractals Qt5::Widgets Qt5::Core Qt5::Network
        Qt5::Sql Qt5::OpenGL
        ${Boost_LIBRARIES} )