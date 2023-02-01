#pragma once

/* This header file contains every header in core/state_evaluator.
 * This should only be used if every header file in core/state_evaluator is relevant.
 * i.e., when allowing arbitrary StateEvaluator construction.
 *
 * Also contains enum relevant to StateEvaluator types
 */

#include "state_evaluator.h"

#include "equal_state_evaluator.h"
#include "greater_than_state_evaluator.h"
#include "greater_than_equal_state_evaluator.h"
#include "less_than_state_evaluator.h"
#include "less_than_equal_state_evaluator.h"
#include "in_range_state_evaluator.h"

typedef enum {
    InRange,
    Greater,
    Less,
    GreaterOrEqual,
    LessOrEqual,
    Equal,
} StateEvaluatorType;

Q_DECLARE_METATYPE(StateEvaluatorType)

