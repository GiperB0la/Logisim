#pragma once
#include "IGate.hpp"


class InputGate : public IGate
{
public:
    InputGate(const sf::Font& font) {
        shape.setSize({ 30.f, 30.f });
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.f);
        shape.setFillColor(value ? sf::Color::Green : sf::Color::Red);

        name.setFont(font);
        name.setString("In");
        name.setCharacterSize(14);
        name.setFillColor(sf::Color::Black);
        name.setPosition(sf::Vector2f());
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
        window.draw(name);

        for (auto& pos : getOutputPositions()) {
            sf::CircleShape pin(3.f);
            pin.setFillColor(sf::Color::Blue);
            pin.setOrigin(3.f, 3.f);
            pin.setPosition(pos);
            window.draw(pin);
        }
    }

    void toggle() {
        value = !value;
        outputs[0] = value;
        shape.setFillColor(value ? sf::Color::Green : sf::Color::Red);
    }

    std::vector<sf::Vector2f> getOutputPositions() const override {
        sf::Vector2f pos = shape.getPosition();
        float w = shape.getSize().x;
        float h = shape.getSize().y;
        return { { pos.x + w, pos.y + h / 2.f } };
    }

    std::vector<bool> getOutputs() const override {
        return outputs;
    }

    size_t getOutputsCount() const override {
        return outputs.size();
    };

private:
    bool value = false;
    std::vector<bool> outputs{ false };
};