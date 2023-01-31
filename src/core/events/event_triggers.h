#pragma once

#include <cstdlib>

#include <QApplication>
#include <QObject>
#include <QStyle>
#include <QSystemTrayIcon>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <powrprof.h>

#endif

class EventTriggers : public QObject {
public:
    typedef enum {
        Shutdown,
        Hibernate,
        Sleep,
        Notify,
        Shell
    } Action;

    static void triggerEvent(EventTriggers::Action action, std::string shellCommand = "");

    static void showNotification();
};


Q_DECLARE_METATYPE(EventTriggers::Action)
