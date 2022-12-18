cmake_minimum_required(VERSION 3.23)
project(open_smart_shutdown)

set(QT6_INSTALL_PATH D:/pkg/Qt/6.3.1/mingw_64) # Ignore this if CMake can find Qt6 normally
# note that this is my personal path to Qt6 and it is likely different on your system

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

include_directories(core/state_monitor gui)

set(CMAKE_PREFIX_PATH ${QT6_INSTALL_PATH})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(state_monitor_files
        core/state_monitor/state_monitor_manager.cpp
        core/state_monitor/state_monitor_manager.h
        core/state_monitor/state.cpp
        core/state_monitor/state.h)

set(state_evaluator_files
        core/state_monitor/state_evaluator/state_evaluator.h
        core/state_monitor/state_evaluator/in_range_state_evaluator.tpp
        core/state_monitor/state_evaluator/in_range_state_evaluator.h)

set(state_reader_files
        core/state_monitor/state_reader/state_reader.h
        core/state_monitor/state_monitor.tpp
        core/state_monitor/state_reader/time_state_reader.cpp
        core/state_monitor/state_reader/time_state_reader.h)

set(triggered_executions_files
        core/state_monitor/events/event_triggers.cpp
        core/state_monitor/events/event_triggers.h)

set(gui_files
        gui/main.cpp
        gui/main_window.cpp
        gui/main_window.h
        gui/main_state_monitor_widget.cpp
        gui/main_state_monitor_widget.h
        gui/state_monitor_widget.cpp
        gui/state_monitor_widget.h gui/state_monitor_creator_widget.cpp gui/state_monitor_creator_widget.h)


add_executable(open_smart_shutdown
        ${state_monitor_files}
        ${state_evaluator_files}
        ${state_reader_files}
        ${triggered_executions_files}
        ${gui_files})

target_link_libraries(open_smart_shutdown
        Qt::Core
        Qt::Gui
        Qt::Widgets)

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
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT6_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
