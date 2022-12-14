#pragma once

#include <QObject>
#include <QString>
#include <utility>

class State : public QObject {
Q_OBJECT
public:
    bool getState() const {
        return state;
    }

    void setState(bool newState) {
        state = newState;
    }

private:
    bool state = false;

    friend class StateMonitorManager;
};
