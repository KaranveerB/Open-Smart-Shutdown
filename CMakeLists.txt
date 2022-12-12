cmake_minimum_required(VERSION 3.23)
project(open_smart_shutdown)

set(CMAKE_CXX_STANDARD 17)

include_directories(core/state_monitor gui)

set(state_monitor_files
        core/state_monitor/state_monitor_manager.cpp core/state_monitor/state_monitor_manager.h)

set(state_evaluator_files
        core/state_monitor/state_evaluator/state_evaluator.h
        core/state_monitor/state_evaluator/in_range_state_evaluator.cpp
        core/state_monitor/state_evaluator/in_range_state_evaluator.h)

set(state_reader_files
        core/state_monitor/state_reader/state_reader.h
        core/state_monitor/state_monitor.tpp
        core/state_monitor/state_reader/time_state_reader.cpp
        core/state_monitor/state_reader/time_state_reader.h)

set(gui_files
        gui/main.cpp)


add_executable(open_smart_shutdown
        ${state_monitor_files}
        ${state_evaluator_files}
        ${state_reader_files}
        ${gui_files}
        )
