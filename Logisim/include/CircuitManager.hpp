#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "../Gates/include/IGate.hpp"
#include "../Gates/include/AndGate.hpp"
#include "../Gates/include/OrGate.hpp"
#include "../Gates/include/XorGate.hpp"
#include "../Gates/include/NandGate.hpp"
#include "../Gates/include/NorGate.hpp"
#include "../Gates/include/XnorGate.hpp"
#include "../Gates/include/InputGate.hpp"
#include "../Gates/include/OutputGate.hpp"


struct Wire
{
    std::shared_ptr<IGate> fromGate;
    int fromPinIndex;
    std::shared_ptr<IGate> toGate;
    int toPinIndex;
    bool isSelected = false;
};

class CircuitManager
{
public:
    void addGate(std::shared_ptr<IGate> gate);
    void update();
    void draw(sf::RenderWindow& window);
    void handleMousePress(sf::Vector2f pos, bool isShiftHeld);
    void handleMouseRelease();
    void handleMouseDrag(sf::Vector2f pos, const sf::Vector2f& workplacePosition, const sf::Vector2f& workplaceSize);
    void createAndGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createOrGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createXorGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createNandGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createNorGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createXnorGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createInputGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void createOutputGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots);
    void deleteGates();

private:
    std::vector<std::shared_ptr<IGate>> gates;
    std::vector<std::shared_ptr<IGate>> selectedGates;
    sf::Vector2f dragOffset;
    std::unordered_map<std::shared_ptr<IGate>, sf::Vector2f> dragOffsets;
    bool isDragging = false;
    bool isSelecting = false;
    sf::Vector2f selectionStart;
    sf::FloatRect selectionRect;

    std::vector<Wire> wires;
    std::shared_ptr<IGate> draggingFromGate = nullptr;
    int draggingFromPinIndex = -1;
    sf::Vector2f currentMousePos;
    bool isDraggingWire = false;
};