cmake_minimum_required(VERSION 2.8.3)

project(boostSeriport)

add_compile_options(-std=c++11)

ADD_DEFINITIONS(_DANGELA)
ADD_DEFINITIONS(-DNDEBUG)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Designer REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5UiTools REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5UiPlugin REQUIRED)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(./include)
INCLUDE_DIRECTORIES(${Qt5Designer_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Xml_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5UiTools_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5UiPlugin_INCLUDE_DIRS})

set(boostSeriport_SRCS
    src/boostSeriport.cpp
    src/main.cpp
    src/robotwheel.cpp
    src/SerialPortDevice.cpp
    src/udpserver.cpp
    )
    
set(boostSeriport_HDRS
    include/boostSeriport.h
    include/DataGram.h
    include/robotwheel.h
    include/SerialPortDevice.h
    include/udpserver.h
    )
    
set(boostSeriport_UIS
    src/boostSeriport.ui
    )
    
set(boostSeriport_INCLUDE_DIRECTOROES
    include
    )

qt5_wrap_cpp(boostSeriport_MOCS ${boostSeriport_HDRS})
qt5_wrap_ui(boostSeriport_UIS_H ${boostSeriport_UIS})

include_directories(${boostSeriport_INCLUDE_DIRECTOROES})
add_library(${PROJECT_NAME} SHARED ${boostSeriport_SRCS} ${boostSeriport_MOCS} ${boostSeriport_UIS_H}
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Designer
    Qt5::Xml Qt5::UiTools Qt5::UiPlugin)

