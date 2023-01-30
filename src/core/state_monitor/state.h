#pragma once

#include <QObject>
#include <QString>
#include <utility>

class State : public QObject {
Q_OBJECT
public:
    typedef enum {
        Inactive,
        Active,
        Buffered,
        Waiting // TODO: Integrate this
    } StateStatus;

    State(unsigned int baseBufferCount = 3) : baseBufferCount(baseBufferCount) {}

    StateStatus getState() const {
        return state;
    }

    void update(bool isActive) {
        if (isActive && state != StateStatus::Active) {
            state = StateStatus::Active;
            bufferCount = baseBufferCount;
        } else if (!isActive && state != StateStatus::Inactive) {
            if (baseBufferCount > 0) {
                state = StateStatus::Buffered;
                bufferCount--;
                if (bufferCount == 0) {
                    state = StateStatus::Inactive;
                }
            } else {
                state = StateStatus::Inactive;
            }
        }
    }

    std::string getStateValueString() const {
        return stateValueString;
    }

    void setStateValueString(std::string newStateValueString) {
        stateValueString = std::move(newStateValueString);
    }

private:
    unsigned int baseBufferCount;
    unsigned int bufferCount;

    StateStatus state = Inactive;

    std::string stateValueString;
};
