cmake_minimum_required(VERSION 3.12)
project(facefull-bridge VERSION 1.0.3 DESCRIPTION "Facefull bridge library")

set(CMAKE_CXX_STANDARD 14)

set(TARGET_SOURCE
        src/interface.cpp
        include/facefull/bridge/interface.h
        include/facefull/bridge/wx.hpp
        include/facefull/bridge/qt5webkit.hpp
        include/facefull/bridge/web.hpp)

add_library(objlib OBJECT ${TARGET_SOURCE})
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE ON)

OPTION(FFB_WXWIDGETS_EXAMPLE_BUILD "Build Facefull bridge wxWidgets example" ON)
OPTION(FFB_QT5WEBKIT_EXAMPLE_BUILD "Build Facefull bridge Qt5WebKit example" ON)
OPTION(FFB_WB_EXAMPLE_BUILD "Build Facefull web bridge example" ON)

if (WIN32)
    set(FFB_QT5WEBKIT_EXAMPLE_BUILD OFF)
endif()

function(build_ffb LIBTYPE)
    message("Building ${LIBTYPE} variant")

    if(${LIBTYPE} STREQUAL shared)
        set(LIBTYPE "")
        add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
    elseif(${LIBTYPE} STREQUAL static)
        set(LIBTYPE "_static")
        add_library(${PROJECT_NAME}${LIBTYPE} STATIC $<TARGET_OBJECTS:objlib>)
    endif()

    set_target_properties(${PROJECT_NAME}${LIBTYPE} PROPERTIES
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR})

    include_directories(include/)
    include_directories(deps/cpp-httplib)
endfunction()

build_ffb(shared)
build_ffb(static)

if (NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dist)
endif ()

if (WIN32)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_DIR}/lib NAMELINK_COMPONENT)
else()
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${INSTALL_DIR}/lib NAMELINK_COMPONENT)
endif()

install(TARGETS ${PROJECT_NAME}_static ARCHIVE DESTINATION ${INSTALL_DIR}/lib NAMELINK_COMPONENT)
install(DIRECTORY include DESTINATION ${INSTALL_DIR})

if (FFB_WXWIDGETS_EXAMPLE_BUILD OR FFB_QT5WEBKIT_EXAMPLE_BUILD OR FFB_WB_EXAMPLE_BUILD)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui/ DESTINATION ${INSTALL_DIR}/ui)

    add_custom_command(
            TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/deps/facefull/dist/
            ${CMAKE_CURRENT_SOURCE_DIR}/examples/ui/facefull
            COMMENT "Copy Facefull frontend library files"
    )

    if (FFB_WXWIDGETS_EXAMPLE_BUILD)
        add_subdirectory(examples/wxwidgets)
    endif()

    if (FFB_QT5WEBKIT_EXAMPLE_BUILD)
        add_subdirectory(examples/qt5webkit)
    endif()

    if (FFB_WB_EXAMPLE_BUILD)
        add_subdirectory(examples/web_bridge)
    endif()
endif()
