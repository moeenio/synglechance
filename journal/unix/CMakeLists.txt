cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(OneShot_Journal VERSION 1.2.0 LANGUAGES CXX)
set(EXECUTABLE_NAME "_______")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

## Setup options ##

option(DEBUG "Debug mode" OFF)

## Misc setup ##

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep")

## Setup main source ##

set(PROJECT_SOURCES
        main.cpp
        pixmap.cpp
        pixmap.h
        pipe.cpp
        pipe.h
        journal.cpp
        journal.h
        niko.cpp
        niko.h
)

add_executable(${EXECUTABLE_NAME}
    ${PROJECT_SOURCES}
)

## Link packages ##

target_link_libraries(${EXECUTABLE_NAME}
	CONAN_PKG::boost
	CONAN_PKG::qt
	${PLATFORM_LIBRARIES})

## Make bundle ##

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE

    MACOSX_BUNDLE_GUI_IDENTIFIER gooborgstudios.${PROJECT_NAME}.osx
    MACOSX_BUNDLE_BUNDLE_NAME ${EXECUTABLE_NAME}
    MACOSX_BUNDLE_LONG_VERSION_STRING "macOS Port v.${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_INFO_STRING "You only have one shot."
    # MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/patches/mac/JournalInfo.plist.in
    MACOSX_BUNDLE_ICON_FILE "icon"
    MACOSX_BUNDLE_COPYRIGHT "Unix Journal for OneShot Copyright © 2022 Gooborg Studios, MIT license. OneShot Copyright © 2017-2019 OneShot team, all rights reserved. View CREDITS.txt for more."
)
