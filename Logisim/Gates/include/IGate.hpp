#pragma once
#include <SFML/Graphics.hpp>


class IGate
{
public:
    IGate() {
        id = globalGateIdCounter++;
    };
    virtual ~IGate() = default;

    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void update() {};

    virtual std::vector<sf::Vector2f> getInputPositions() const {
        return {};
    }

    virtual std::vector<sf::Vector2f> getOutputPositions() const {
        return {};
    }

    virtual std::vector<bool> getOutputs() const {
        return {};
    };

    virtual std::vector<bool> getInputs() const {
        return {};
    };

    virtual void setInputs(const std::vector<bool>& in) {}

    virtual size_t getInputsCount() const {
        return 0;
    };

    virtual size_t getOutputsCount() const {
        return 0;
    };

    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);

        sf::FloatRect textBounds = name.getLocalBounds();
        sf::Vector2f shapeSize = shape.getSize();

        name.setPosition(
            pos.x + (shapeSize.x - textBounds.width) / 2.f - textBounds.left,
            pos.y + (shapeSize.y - textBounds.height) / 2.f - textBounds.top
        );
    }

    sf::Vector2f getPosition() const {
        return shape.getPosition();
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

    bool containsPoint(sf::Vector2f point) const {
        return shape.getGlobalBounds().contains(point);
    }

    void setSelected(bool selected) {
        isSelected = selected;
        shape.setFillColor(isSelected ? sf::Color(168, 208, 240) : sf::Color::White);
    }

    bool isSelectedNow() const {
        return isSelected;
    }

    sf::Vector2f getSize() const {
        return shape.getSize();
    }

public:
    size_t id;
    static size_t globalGateIdCounter;

protected:
    sf::Text name;
    sf::RectangleShape shape;
    bool isSelected = false;
};