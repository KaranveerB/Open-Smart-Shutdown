cmake_minimum_required(VERSION 3.23)
project(open_smart_shutdown)

# Personal Qt6 path. Likely different on other systems. Safe to ignore if CMake can find Qt6 normally
set(QT6_INSTALL_PATH D:/pkg/Qt/6.3.1/msvc2019_64)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH ${QT6_INSTALL_PATH})

if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "No build type specified. Defaulting to Debug")
        set(CMAKE_BUILD_TYPE "Debug")
endif()

include_directories(core gui wmi/include wmi-extended)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        REQUIRED)

add_subdirectory(wmi)

file(GLOB state_monitor_files
        core/state_monitor/*.h
        core/state_monitor/*.cpp)
file(GLOB state_evaluator_files
        core/state_monitor/state_evaluator/*.h)
file(GLOB state_reader_files
        core/state_monitor/state_reader/*.h)
file(GLOB system_monitor_files
        core/system_monitor/*.h
        core/system_monitor/*.cpp)
file(GLOB triggered_executions_files
        core/events/*.h
        core/events/*.cpp)
file(GLOB gui_files
        gui/*.h
        gui/*.cpp
        gui/*.qrc)
if (WIN32)
file(GLOB wmi_files
        wmi/include/*.hpp
        wmi-extended/*.hpp)
endif()

add_executable(open_smart_shutdown
        main.cpp
        ${state_monitor_files}
        ${state_evaluator_files}
        ${state_reader_files}
        ${system_monitor_files}
        ${triggered_executions_files}
        ${gui_files}
        ${gui_resource_files}
        ${wmi_files})

target_link_libraries(open_smart_shutdown
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg)

if (WIN32)
    target_link_libraries(open_smart_shutdown powrprof wmi)
    add_executable(GuiApplication WIN32 main.cpp)
    set_target_properties(open_smart_shutdown PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

set_target_properties(open_smart_shutdown PROPERTIES OUTPUT_NAME "OpenSmartShutdown")

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    if (NOT EXISTS "${QT6_INSTALL_PATH}/bin")
        set(QT6_INSTALL_PATH "${QT6_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT6_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT6_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT6_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT6_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB core gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT6_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
