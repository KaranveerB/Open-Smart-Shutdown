#include "state.h"

State::State(unsigned int baseBufferCount) : baseBufferCount(baseBufferCount), bufferCount(baseBufferCount) {}

State::StateStatus State::getState() const {
	return state;
}

void State::update(bool isActive) {
	if (isActive && state != StateStatus::Active) {
		state = StateStatus::Active;
		bufferCount = baseBufferCount;
	} else if (!isActive && state != StateStatus::Inactive) {
		if (baseBufferCount > 0 && state != StateStatus::Waiting && state != StateStatus::Error) {
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

std::string State::getStateValueString() const {
	return stateValueString;
}

void State::setStateValueString(std::string newStateValueString) {
	stateValueString = std::move(newStateValueString);
}

unsigned int State::getBufferCount() const {
	return bufferCount;
}

void State::scheduleForDeletion() {
	scheduledForDeletion = true;
}

bool State::isScheduledForDeletion() {
	return scheduledForDeletion;
}

void State::setError() {
	state = Error;
}
