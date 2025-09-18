#pragma once
#include "IGate.hpp"


class OutputGate : public IGate
{
public:
    OutputGate(const sf::Font& font) {
        shape.setSize({ 30.f, 30.f });
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.f);
        shape.setFillColor(value ? sf::Color::Green : sf::Color::Red);

        name.setFont(font);
        name.setString("Out");
        name.setCharacterSize(14);
        name.setFillColor(sf::Color::Black);
        name.setPosition(sf::Vector2f());
    }

    void update() override {
        if (!inputs.empty()) {
            value = inputs[0];
            shape.setFillColor(value ? sf::Color::Green : sf::Color::Red);
        }
    }

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
    }

    void setInputs(const std::vector<bool>& in) override {
        inputs = in;
    }

    std::vector<sf::Vector2f> getInputPositions() const override {
        sf::Vector2f pos = shape.getPosition();
        float h = shape.getSize().y;
        return {
            { pos.x, pos.y + h / 2.f }
        };
    }

    std::vector<bool> getInputs() const override {
        return inputs;
    }

    size_t getInputsCount() const override {
        return inputs.size();
    };

private:
    bool value = false;
    std::vector<bool> inputs{ false };
};