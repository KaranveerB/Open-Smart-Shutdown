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

    std::string getStateValueString() const {
        return stateValueString;
    }

    void setStateValueString(std::string newStateValueString) {
        stateValueString = std::move(newStateValueString);
    }

private:
    bool state = false; // TODO: Make state thread safe
    std::string stateValueString;

    friend class StateMonitorManager;
};
