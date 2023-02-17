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
        Waiting,
        Error
    } StateStatus;

    State(unsigned int baseBufferCount = 3);

    StateStatus getState() const;

    void update(bool isActive);

    std::string getStateValueString() const;

    void setStateValueString(std::string newStateValueString);

    unsigned int getBufferCount() const;

    void scheduleForDeletion();

    bool isScheduledForDeletion();

    void setError();

private:
	StateStatus state = Waiting;
	std::string stateValueString;

    unsigned int baseBufferCount;
    unsigned int bufferCount;

    bool scheduledForDeletion = false;
};
