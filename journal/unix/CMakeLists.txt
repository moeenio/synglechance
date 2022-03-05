cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

Project(OneShot_Journal VERSION 1.2.0)
set(EXECUTABLE_NAME "_______")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

## Setup options ##

option(DEBUG "Debug mode" OFF)

## Misc setup ##

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep")

## Setup main source ##

set(MAIN_HEADERS
	journal.h
	niko.h
)

set(MAIN_SOURCE
	main.cpp
	journal.cpp
	niko.cpp
)

## Setup main executable ##

set(CMAKE_CXX_STANDARD 11)

add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE WIN32 ${MAIN_SOURCE} ${MAIN_HEADERS})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

IF(APPLE)
	set_target_properties(${EXECUTABLE_NAME} PROPERTIES
		BUNDLE True
		MACOSX_BUNDLE_GUI_IDENTIFIER gooborgstudios.${PROJECT_NAME}.osx
		MACOSX_BUNDLE_BUNDLE_NAME ${EXECUTABLE_NAME}
		MACOSX_BUNDLE_LONG_VERSION_STRING "macOS Port v.${PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
		MACOSX_BUNDLE_INFO_STRING "You only have one shot."
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/patches/mac/JournalInfo.plist.in
		MACOSX_BUNDLE_ICON_FILE "icon"
		MACOSX_BUNDLE_COPYRIGHT "Unix Journal for OneShot Copyright © 2022 Gooborg Studios, MIT license. OneShot Copyright © 2017-2019 OneShot team, all rights reserved. View CREDITS.txt for more."
	)
ENDIF()
