#include "event_triggers.h"

void EventTriggers::triggerEvent(EventTriggers::Action action, std::string shellCommand) {
    switch (action) {
        case Shutdown:
        case Sleep:
        case Hibernate:
        case Notify: {
            // TODO: Not working
            auto *trayIcon = new QSystemTrayIcon();
            trayIcon->showMessage("Notification Title", "Notification Message", QSystemTrayIcon::NoIcon, 10000);
        }
            break;
        case Shell:
            break;
    }
}
