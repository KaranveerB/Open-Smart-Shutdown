#pragma once

#include <array>
#include <stdexcept>
#include <string>

#include "state_reader.h"

class FloatShellStateReader : public StateReader<float> {
public:
    explicit FloatShellStateReader(std::string shellCommand) : shellCommand(std::move(shellCommand)) {};

    float getStateValue() const override {
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
        float float_result = std::stof(result);
        return float_result;
    };

    std::string stateValueToString(float value) const override {
        return std::to_string(value);
    }

private:
    std::string shellCommand;
};
