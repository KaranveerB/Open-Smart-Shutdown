#include "event_triggers.h"

void EventTriggers::triggerEvent(EventTriggers::Action action, std::string shellCommand) {
    switch (action) {
        case Shutdown: {
#ifdef WIN32
            // We do an aggressive (but graceful) shutdown becuase this should reliably shut down, even if user is afk
            ::InitiateShutdown(NULL, NULL, 30, SHUTDOWN_POWEROFF | SHUTDOWN_FORCE_SELF | SHUTDOWN_FORCE_OTHERS,
                               SHTDN_REASON_FLAG_PLANNED);
#else
#error unsupported platform for shutdowns which is not handled yet
#endif

        }
        case Sleep: {
#ifdef WIN32
            ::SetSuspendState(false, false, false);
#else
#error unsupported platform for shutdowns which is not handled yet
#endif
        }
        case Hibernate: {
#ifdef WIN32
            ::SetSuspendState(true, false, false);
#else
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
    trayIcon.showMessage("Open Smart Shutdown Triggered", "The conditions set have been met and "
                                                          "Open Smart Shutdown has triggered.",
                         QSystemTrayIcon::Information, 10000);
    trayIcon.deleteLater();
}

bool EventTriggers::acquireShutdownPrivilege() {
#ifdef WIN32
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    bool result = false;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        result = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
        CloseHandle(hToken);
    }
    return result;
#else
#error system not supported for shutdown
#endif
}