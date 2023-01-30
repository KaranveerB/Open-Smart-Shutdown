#pragma once

#include <QObject>
#include <QSystemTrayIcon>

class EventTriggers {
public:
    typedef enum {
        Shutdown,
        Hibernate,
        Sleep,
        Notify,
        Shell
    } Action;

    static void triggerEvent(EventTriggers::Action action, std::string shellCommand = "");
};


Q_DECLARE_METATYPE(EventTriggers::Action)
