#pragma once

/* This header file contains every header in core/state_reader.
 * This should only be used if every header file in core/state_reader is relevant.
 * i.e., when allowing arbitrary StateReader construction.
 *
 * Also contains enum relevant to StateReader types
 */

#include "state_reader.h"
#include "time_state_reader.h"

typedef enum {
    Time,
    Cpu,
    Disk,
    Network,
    Shell
} StateReaderType;

Q_DECLARE_METATYPE(StateReaderType)

