#include "../include/CircuitManager.hpp"


size_t IGate::globalGateIdCounter = 0;

void CircuitManager::addGate(std::shared_ptr<IGate> gate) 
{
    gates.push_back(gate);
}

void CircuitManager::update() 
{
    for (auto& gate : gates) {
        std::vector<bool> inputs(gate->getInputsCount(), false);
        gate->setInputs(inputs);
    }

    for (const auto& wire : wires) {
        auto outputs = wire.fromGate->getOutputs();
        if (wire.fromPinIndex < outputs.size()) {
            bool signal = outputs[wire.fromPinIndex];

            auto currentInputs = wire.toGate->getInputs();
            if (wire.toPinIndex < currentInputs.size()) {
                currentInputs[wire.toPinIndex] = signal;
                wire.toGate->setInputs(currentInputs);
            }
        }
    }

    for (auto& gate : gates) {
        gate->update();
    }
}

void CircuitManager::draw(sf::RenderWindow& window) 
{
    for (auto& gate : gates) {
        gate->draw(window);
    }

    if (isSelecting) {
        sf::RectangleShape rectShape;
        rectShape.setPosition(selectionRect.left, selectionRect.top);
        rectShape.setSize({ selectionRect.width, selectionRect.height });
        rectShape.setFillColor(sf::Color(0, 120, 215, 50));
        rectShape.setOutlineColor(sf::Color(0, 120, 215));
        rectShape.setOutlineThickness(1.f);
        window.draw(rectShape);
    }

    for (const auto& wire : wires) {
        sf::Color color = wire.isSelected ? sf::Color::Red : sf::Color::Black;

        sf::Vertex line[] = {
            sf::Vertex(wire.fromGate->getOutputPositions()[wire.fromPinIndex], color),
            sf::Vertex(wire.toGate->getInputPositions()[wire.toPinIndex], color)
        };
        window.draw(line, 2, sf::Lines);
    }

    if (isDraggingWire && draggingFromGate) {
        sf::Vertex tempLine[] = {
            sf::Vertex(draggingFromGate->getOutputPositions()[draggingFromPinIndex], sf::Color::Red),
            sf::Vertex(currentMousePos, sf::Color::Red)
        };
        window.draw(tempLine, 2, sf::Lines);
    }
}

void CircuitManager::handleMousePress(sf::Vector2f pos, bool isShiftHeld)
{
    bool clickedOnSelected = false;
    std::shared_ptr<IGate> clickedGate = nullptr;

    for (auto& wire : wires) {
        bool clickedOnPin = false;

        for (auto& gate : gates) {
            auto outputPositions = gate->getOutputPositions();
            for (size_t i = 0; i < outputPositions.size(); ++i) {
                float dist = std::hypot(pos.x - outputPositions[i].x, pos.y - outputPositions[i].y);
                if (dist < 10.f) {
                    draggingFromGate = gate;
                    draggingFromPinIndex = static_cast<int>(i);
                    isDraggingWire = true;
                    currentMousePos = pos;
                    clickedOnPin = true;
                    return;
                }
            }
        }

        if (!clickedOnPin) {
            for (auto& wire : wires) {
                sf::Vector2f p1 = wire.fromGate->getOutputPositions()[wire.fromPinIndex];
                sf::Vector2f p2 = wire.toGate->getInputPositions()[wire.toPinIndex];

                float dx = p2.x - p1.x;
                float dy = p2.y - p1.y;
                float lengthSquared = dx * dx + dy * dy;

                if (lengthSquared == 0.f)
                    continue;

                float t = std::max(0.f, std::min(1.f, ((pos.x - p1.x) * dx + (pos.y - p1.y) * dy) / lengthSquared));
                float closestX = p1.x + t * dx;
                float closestY = p1.y + t * dy;

                float distance = std::hypot(pos.x - closestX, pos.y - closestY);

                if (distance < 5.f) {
                    wire.isSelected = !wire.isSelected;
                    return;
                }
            }
        }
    }

    for (auto& gate : gates) {
        if (gate->containsPoint(pos)) {
            clickedGate = gate;
            if (std::find(selectedGates.begin(), selectedGates.end(), gate) != selectedGates.end()) {
                clickedOnSelected = true;
            }
            break;
        }
    }

    for (auto& gate : gates) {
        auto outputPositions = gate->getOutputPositions();
        for (size_t i = 0; i < outputPositions.size(); ++i) {
            float dist = std::hypot(pos.x - outputPositions[i].x, pos.y - outputPositions[i].y);
            if (dist < 10.f) {
                draggingFromGate = gate;
                draggingFromPinIndex = static_cast<int>(i);
                isDraggingWire = true;
                currentMousePos = pos;
                return;
            }
        }
    }

    if (!clickedGate && !isShiftHeld) {
        for (auto& selected : selectedGates)
            selected->setSelected(false);
        selectedGates.clear();

        isSelecting = true;
        selectionStart = pos;
        selectionRect = sf::FloatRect(pos.x, pos.y, 0.f, 0.f);
        return;
    }

    if (clickedGate && !isShiftHeld && !clickedOnSelected) {
        for (auto& selected : selectedGates)
            selected->setSelected(false);
        selectedGates.clear();
    }

    if (clickedGate) {
        if (isShiftHeld) {
            auto it = std::find(selectedGates.begin(), selectedGates.end(), clickedGate);
            if (it != selectedGates.end()) {
                clickedGate->setSelected(false);
                selectedGates.erase(it);
            }
            else {
                clickedGate->setSelected(true);
                selectedGates.push_back(clickedGate);
            }
        }
        else if (!clickedOnSelected) {
            clickedGate->setSelected(true);
            selectedGates.push_back(clickedGate);
        }

        dragOffsets.clear();
        for (auto& selected : selectedGates) {
            dragOffsets[selected] = pos - selected->getPosition();
        }
        isDragging = true;

        if (auto* input = dynamic_cast<InputGate*>(clickedGate.get())) {
            input->toggle();
        }
    }
}

void CircuitManager::handleMouseRelease() 
{
    if (isSelecting) {
        for (auto& gate : gates) {
            sf::Vector2f gatePos = gate->getPosition();
            sf::Vector2f gateSize = gate->getSize();
            sf::FloatRect gateRect(gatePos.x, gatePos.y, gateSize.x, gateSize.y);

            if (selectionRect.intersects(gateRect)) {
                gate->setSelected(true);
                selectedGates.push_back(gate);
            }
        }

        isSelecting = false;
    }

    else if (isDragging) {
        dragOffsets.clear();
        isDragging = false;
    }

    if (isDraggingWire && draggingFromGate) {
        for (auto& gate : gates) {
            auto inputPositions = gate->getInputPositions();
            for (size_t i = 0; i < inputPositions.size(); ++i) {
                float dist = std::hypot(currentMousePos.x - inputPositions[i].x, currentMousePos.y - inputPositions[i].y);
                if (dist < 10.f) {
                    wires.push_back({ draggingFromGate, draggingFromPinIndex, gate, static_cast<int>(i) });
                    break;
                }
            }
        }

        isDraggingWire = false;
        draggingFromGate = nullptr;
        draggingFromPinIndex = -1;
    }
}

void CircuitManager::handleMouseDrag(sf::Vector2f pos, const sf::Vector2f& workplacePosition, const sf::Vector2f& workplaceSize)
{
    if (isSelecting) {
        float left = std::clamp(std::min(selectionStart.x, pos.x), workplacePosition.x, workplacePosition.x + workplaceSize.x);
        float top = std::clamp(std::min(selectionStart.y, pos.y), workplacePosition.y, workplacePosition.y + workplaceSize.y);
        float right = std::clamp(std::max(selectionStart.x, pos.x), workplacePosition.x, workplacePosition.x + workplaceSize.x);
        float bottom = std::clamp(std::max(selectionStart.y, pos.y), workplacePosition.y, workplacePosition.y + workplaceSize.y);

        selectionRect = sf::FloatRect(left, top, right - left, bottom - top);
    }
    else if (isDragging) {
        float spacingX = 10.f;
        float spacingY = 10.f;

        float baseX = workplacePosition.x + 1.f;
        float baseY = workplacePosition.y + 1.f;

        for (auto& [gate, offset] : dragOffsets) {
            sf::Vector2f rawPos = pos - offset;

            float localX = rawPos.x - baseX;
            float localY = rawPos.y - baseY;

            float snappedX = baseX + std::round(localX / spacingX) * spacingX;
            float snappedY = baseY + std::round(localY / spacingY) * spacingY;

            sf::Vector2f gateSize = gate->getSize();

            float maxX = baseX + workplaceSize.x - gateSize.x;
            float maxY = baseY + workplaceSize.y - gateSize.y;

            snappedX = std::clamp(snappedX, baseX, maxX);
            snappedY = std::clamp(snappedY, baseY, maxY);

            gate->setPosition({ snappedX, snappedY });
        }
    }

    if (isDraggingWire) {
        currentMousePos = pos;
    }
}

void CircuitManager::createAndGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<AndGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createOrGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<OrGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createXorGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<XorGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createNandGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<NandGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createNorGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<NorGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createXnorGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<XnorGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createInputGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<InputGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::createOutputGateAt(sf::Vector2f pos, const sf::Font& font, const std::vector<sf::CircleShape>& gridDots)
{
    if (gridDots.empty()) {
        return;
    }

    auto gate = std::make_shared<OutputGate>(font);

    sf::Vector2f dotCenter = gridDots.front().getPosition() + sf::Vector2f(gridDots.front().getRadius(), gridDots.front().getRadius());

    gate->setPosition(dotCenter);

    gates.push_back(gate);
}

void CircuitManager::deleteGates()
{
    wires.erase(
        std::remove_if(wires.begin(), wires.end(),
            [this](const Wire& wire) {
                return std::find(selectedGates.begin(), selectedGates.end(), wire.fromGate) != selectedGates.end() ||
                    std::find(selectedGates.begin(), selectedGates.end(), wire.toGate) != selectedGates.end();
            }),
        wires.end()
    );

    gates.erase(
        std::remove_if(gates.begin(), gates.end(),
            [this](const std::shared_ptr<IGate>& gate) {
                return std::find(selectedGates.begin(), selectedGates.end(), gate) != selectedGates.end();
            }),
        gates.end()
    );

    wires.erase(
        std::remove_if(wires.begin(), wires.end(),
            [](const Wire& wire) { return wire.isSelected; }),
        wires.end()
    );

    selectedGates.clear();
}