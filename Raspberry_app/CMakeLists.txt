cmake_minimum_required(VERSION 3.10)
project(Raspberry_app)

INCLUDE(FindPackageHandleStandardArgs)

message( "Configuring Project Raspberry_app.")

#
# CMake global setup.
#####################

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile Flags.
################
set(STATIC_LINKING FALSE )
set( CMAKE_CXX_STANDARD 17 )


set( CMAKE_CXX_FLAGS "-marm ${CMAKE_CXX_FLAGS}")
set( CMAKE_CXX_FLAGS_DEBUG "-g" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# Project Path.
###############
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


#-----------------------------------------------------------------------------
macro( ABL_EXECUTABLE Target )
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    set( CMAKE_INCLUDE_CURRENT_DIR ON )
    set_property( GLOBAL APPEND PROPERTY ABL_TARGET_LIST ${Target} )

    add_executable( ${Target} WIN32 ${ARGN} )
endmacro()

# Qt setup.
###########
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER generated)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # To find moc files and other generated stuff.
set(CMAKE_PREFIX_PATH "${QT_DIR};${CMAKE_PREFIX_PATH}" )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set( LINUX TRUE )
endif()

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Network REQUIRED )
find_package(raspicam REQUIRED)
find_package(OpenCV REQUIRED)


IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
    MESSAGE(STATUS "COMPILING OPENCV TESTS")


    set(RASPI_INCLUDE_DIR /home/yannk/Bureau/raspi/sysroot/)
    set(RASPICAM_LIB_DIR ${RASPI_INCLUDE_DIR}/usr/local/lib)


    set( Target Raspberry_app )
    abl_executable( ${Target}
            main.cpp
            mainwindow.h mainwindow.cpp
            UDP_Server/UDP_Server.cpp UDP_Server/UDP_Server.h
            UDP_Server/UDP_Worker.cpp UDP_Server/UDP_Worker.h
            )
    target_include_directories(${Target} PUBLIC ${RASPI_INCLUDE_DIR}/usr/local/include/)
    target_link_libraries( ${Target} Qt5::Core Qt5::Widgets Qt5::Network ${raspicam_CV_LIBS})

ELSE()
    MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()



