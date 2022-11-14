cmake_minimum_required(VERSION 3.23)
project(open_smart_shutdown)

set(CMAKE_CXX_STANDARD 14)

include_directories(core/state_monitor gui)

set(state_monitor_files
        core/state_monitor/state_monitor.cpp
        core/state_monitor/state_monitor.h
        )

set(state_parser_files
        core/state_monitor/state_parser/state_parser.h
        core/state_monitor/state_parser/in_range_state_parser.cpp
        core/state_monitor/state_parser/in_range_state_parser.h
        )

set(state_reader_files
        core/state_monitor/state_reader/state_reader.h
        core/state_monitor/state_reader/default_time_state_reader.cpp
        core/state_monitor/state_reader/default_time_state_reader.h
        )

set(gui_files
        gui/main.cpp
        )


add_executable(open_smart_shutdown
        ${state_monitor_files}
        ${state_parser_files}
        ${state_reader_files}
        ${gui_files}
        )
