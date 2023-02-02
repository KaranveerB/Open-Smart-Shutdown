#pragma once

#include <array>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "state_reader.h"

class StringShellStateReader : public StateReader<std::string> {
public:
    explicit StringShellStateReader(std::string shellCommand) : shellCommand(std::move(shellCommand)) {}

    std::string getStateValue() const override {
        std::array<char, 128> buffer{};
        std::string result;
        FILE *pipe = _popen(shellCommand.c_str(), "r");
        if (!pipe) {
            throw std::runtime_error("couldn't execute shell command");
        }
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
        _pclose(pipe);
        result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());
        return result;
    };

    std::string stateValueToString(std::string value) const override {
        return value;
    }

private:
    std::string shellCommand;
};
