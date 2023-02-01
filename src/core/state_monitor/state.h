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
        Waiting
    } StateStatus;

    State(unsigned int baseBufferCount = 3) : baseBufferCount(baseBufferCount), bufferCount(baseBufferCount) {}

    StateStatus getState() const {
        return state;
    }

    void update(bool isActive) {
        if (isActive && state != StateStatus::Active) {
            state = StateStatus::Active;
            bufferCount = baseBufferCount;
        } else if (!isActive && state != StateStatus::Inactive) {
            if (baseBufferCount > 0 && state != StateStatus::Waiting) {
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

    unsigned int getBufferCount() {
        return bufferCount;
    }

    void scheduleForDeletion() {
        scheduledForDeletion = true;
    }

    bool isScheduledForDeletion() {
        return scheduledForDeletion;
    }

private:

    unsigned int baseBufferCount;
    unsigned int bufferCount;

    bool scheduledForDeletion = false;

    StateStatus state = Waiting;

    std::string stateValueString;
};
