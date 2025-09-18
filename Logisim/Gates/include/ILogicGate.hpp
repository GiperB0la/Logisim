#pragma once
#include "IGate.hpp"


class ILogicGate : public IGate
{
public:
    ILogicGate(const sf::Font& font, const std::string& gateName, sf::Vector2f size, size_t inputsCount = 2) {
        shape.setSize(size);
        shape.setFillColor(sf::Color::White);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.f);

        name.setFont(font);
        name.setString(gateName);
        name.setCharacterSize(14);
        name.setFillColor(sf::Color::Black);

        inputs.resize(inputsCount, false);
        outputs.resize(1, false);
    }

    virtual ~ILogicGate() = default;

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
        window.draw(name);

        for (auto& pos : getInputPositions()) {
            sf::CircleShape pin(3.f);
            pin.setFillColor(sf::Color::Red);
            pin.setOrigin(3.f, 3.f);
            pin.setPosition(pos);
            window.draw(pin);
        }

        for (auto& pos : getOutputPositions()) {
            sf::CircleShape pin(3.f);
            pin.setFillColor(sf::Color::Blue);
            pin.setOrigin(3.f, 3.f);
            pin.setPosition(pos);
            window.draw(pin);
        }
    }

    std::vector<sf::Vector2f> getInputPositions() const override {
        std::vector<sf::Vector2f> positions;
        sf::Vector2f pos = shape.getPosition();
        float h = shape.getSize().y;

        size_t inputCount = inputs.size();
        if (inputCount == 1) {
            positions.push_back({ pos.x, pos.y + h / 2.f });
        }
        else {
            for (size_t i = 0; i < inputCount; ++i) {
                float yPos = pos.y + (h / (inputCount + 1)) * (i + 1);
                positions.push_back({ pos.x, yPos });
            }
        }

        return positions;
    }

    std::vector<sf::Vector2f> getOutputPositions() const override {
        sf::Vector2f pos = shape.getPosition();
        float w = shape.getSize().x;
        float h = shape.getSize().y;

        return { { pos.x + w, pos.y + h / 2.f } };
    }

    void setInputs(const std::vector<bool>& in) override {
        inputs = in;
    }

    std::vector<bool> getOutputs() const override {
        return outputs;
    }

    std::vector<bool> getInputs() const override {
        return inputs;
    };

    size_t getInputsCount() const override {
        return inputs.size();
    };

    size_t getOutputsCount() const override {
        return outputs.size();
    };

protected:
    std::vector<bool> inputs;
    std::vector<bool> outputs;
};