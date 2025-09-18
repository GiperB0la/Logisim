#pragma once
#include "ILogicGate.hpp"


class XnorGate : public ILogicGate
{
public:
    XnorGate(const sf::Font& font) : ILogicGate(font, "Xnor", { 60.f, 60.f }) {}

    void update() override {
        if (inputs.empty()) {
            outputs[0] = true;
            return;
        }

        bool result = inputs[0];
        for (size_t i = 1; i < inputs.size(); ++i) {
            result ^= inputs[i];
        }
        outputs[0] = !result;
    }
};