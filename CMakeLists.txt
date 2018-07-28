cmake_minimum_required(VERSION 3.10)

Project(osgbindattr1)
LIST(APPEND CMAKE_MODULE_PATH "./CMakeModules")
FIND_PACKAGE(OSG)

set(CMAKE_DEBUG_POSTFIX "d")

set(OUTPUT_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText)
IF(WIN32)
    find_package(GLCORE)
ENDIF() 

set(app_srcs
	main.cpp
)

set(app_hdrs
)

add_executable (app ${app_srcs} ${app_hdrs})

set_property(TARGET app PROPERTY CXX_STANDARD 11)

IF(WIN32)
	target_include_directories(
		app  PUBLIC
		${GLCORE_INCLUDE_DIR}
		${OSG_INCLUDE_DIR}
	)
ENDIF() 

target_link_libraries(app ${OPENSCENEGRAPH_LIBRARIES})
