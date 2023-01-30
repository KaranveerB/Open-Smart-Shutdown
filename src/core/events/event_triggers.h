#pragma once

#include <QObject>

class EventTriggers {
public:
    typedef enum {
        Shutdown,
        Hibernate,
        Sleep,
        Shell
    } Action;
};


Q_DECLARE_METATYPE(EventTriggers::Action)
