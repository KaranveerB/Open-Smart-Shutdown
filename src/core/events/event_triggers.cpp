#include "event_triggers.h"

void EventTriggers::triggerEvent(EventTriggers::Action action, std::string shellCommand) {
    switch (action) {
        case Shutdown: {
#ifdef WIN32
            // We do an aggressive (but graceful) shutdown becuase this should reliably shut down, even if user is afk
            ::InitiateShutdownW(NULL, NULL, 30, SHUTDOWN_POWEROFF | SHUTDOWN_FORCE_SELF | SHUTDOWN_FORCE_OTHERS,
                                SHTDN_REASON_FLAG_PLANNED);
#elif
#error unsupported platform for shutdowns which is not handled yet
#endif

        }
        case Sleep: {
#ifdef WIN32
            ::SetSuspendState(false, false, false);
#elif
#error unsupported platform for shutdowns which is not handled yet
#endif
        }
        case Hibernate: {
#ifdef WIN32
            ::SetSuspendState(true, false, false);
#elif
#error unsupported platform for shutdowns which is not handled yet
#endif
        }
        case Notify: {
            showNotification();
        }
            break;
        case Shell:
            system(shellCommand.c_str());
            break;
    }
    exit(0); // quit the program when done
}

void EventTriggers::showNotification() {
    QSystemTrayIcon trayIcon;
    QIcon icon = qApp->style()->standardIcon(QStyle::SP_MessageBoxCritical);
    trayIcon.setObjectName("Name");
    trayIcon.setIcon(icon);
    trayIcon.show();
    trayIcon.showMessage("yo", "ay", QSystemTrayIcon::Information, 10000);
    trayIcon.deleteLater();
}