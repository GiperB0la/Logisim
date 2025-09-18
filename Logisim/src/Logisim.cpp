#include "../include/Logisim.hpp"


Logisim::Logisim()
{
    init();
}

void Logisim::init()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    workplaceSize = sf::Vector2f(desktop.getDesktopMode().width, desktop.getDesktopMode().height);
    workplacePosition = sf::Vector2f(249.f, 10.f);

    ctx.antialiasingLevel = 8;
    window.create(desktop, "Logisim", sf::Style::Default, ctx);
    window.setFramerateLimit(60);

    HWND hwnd = window.getSystemHandle();
    ShowWindow(hwnd, SW_MAXIMIZE);

    font.loadFromFile("Graphics\\Fonts\\Roboto.ttf");

    panelT.loadFromFile("Graphics\\Textures\\Panel.png");
    panelS.setTexture(panelT);
    panelS.setPosition(sf::Vector2f(0.f, 0.f));

    workplaceT.loadFromFile("Graphics\\Textures\\Workplace.png");
    workplaceS.setTexture(workplaceT);
    workplaceS.setPosition(sf::Vector2f(239.f, 0.f));

    gridDots.clear();

    float dotRadius = 1.f;
    float spacingX = 10.f;
    float spacingY = 10.f;

    for (float y = 0.f; y <= workplaceSize.y; y += spacingY) {
        for (float x = 0.f; x <= workplaceSize.x; x += spacingX) {
            sf::CircleShape dot(dotRadius);
            dot.setPosition(workplacePosition.x + x, workplacePosition.y + y);
            dot.setFillColor(sf::Color::Black);
            gridDots.push_back(dot);
        }
    }

    createButtons();
}

void Logisim::createButtons()
{
    buttons.emplace_back(
        static_cast<int>(CategoryButton::Cursor),
        sf::Vector2f(10.f, 10.f),
        "Graphics\\Buttons\\Cursor\\Cursor1.png",
        "Graphics\\Buttons\\Cursor\\Cursor2.png",
        "Graphics\\Buttons\\Cursor\\Cursor3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Editing),
        sf::Vector2f(50.f, 10.f),
        "Graphics\\Buttons\\Editing\\Editing1.png",
        "Graphics\\Buttons\\Editing\\Editing2.png",
        "Graphics\\Buttons\\Editing\\Editing3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::And),
        sf::Vector2f(10.f, 62.f),
        "Graphics\\Buttons\\And\\And1.png",
        "Graphics\\Buttons\\And\\And2.png",
        "Graphics\\Buttons\\And\\And3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Or),
        sf::Vector2f(90.f, 62.f),
        "Graphics\\Buttons\\Or\\Or1.png",
        "Graphics\\Buttons\\Or\\Or2.png",
        "Graphics\\Buttons\\Or\\Or3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Xor),
        sf::Vector2f(170.f, 62.f),
        "Graphics\\Buttons\\Xor\\Xor1.png",
        "Graphics\\Buttons\\Xor\\Xor2.png",
        "Graphics\\Buttons\\Xor\\Xor3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Nand),
        sf::Vector2f(10.f, 142.f),
        "Graphics\\Buttons\\Nand\\Nand1.png",
        "Graphics\\Buttons\\Nand\\Nand2.png",
        "Graphics\\Buttons\\Nand\\Nand3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Nor),
        sf::Vector2f(90.f, 142.f),
        "Graphics\\Buttons\\Nor\\Nor1.png",
        "Graphics\\Buttons\\Nor\\Nor2.png",
        "Graphics\\Buttons\\Nor\\Nor3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Xnor),
        sf::Vector2f(170.f, 142.f),
        "Graphics\\Buttons\\Xnor\\Xnor1.png",
        "Graphics\\Buttons\\Xnor\\Xnor2.png",
        "Graphics\\Buttons\\Xnor\\Xnor3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Input),
        sf::Vector2f(65.f, 222.f),
        "Graphics\\Buttons\\Input\\Input1.png",
        "Graphics\\Buttons\\Input\\Input2.png",
        "Graphics\\Buttons\\Input\\Input3.png",
        L"",
        font
    );

    buttons.emplace_back(
        static_cast<int>(CategoryButton::Output),
        sf::Vector2f(145.f, 222.f),
        "Graphics\\Buttons\\Output\\Output1.png",
        "Graphics\\Buttons\\Output\\Output2.png",
        "Graphics\\Buttons\\Output\\Output3.png",
        L"",
        font
    );
}

void Logisim::run()
{
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Logisim::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            window.setView(sf::View(visibleArea));
        }

        // ЛКМ нажата — начать перемещение
        bool shiftHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
            circuit.handleMousePress(mousePos, shiftHeld);
        }

        // ЛКМ отпущена — закончить
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            circuit.handleMouseRelease();
        }

        // Перетаскивание
        if (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y });
            circuit.handleMouseDrag(mousePos, workplacePosition, workplaceSize);
        }

        sf::Vector2f centerPos = workplacePosition + workplaceSize / 2.f;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
            circuit.createAndGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O) {
            circuit.createOrGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
            circuit.createXorGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
            circuit.createNandGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
            circuit.createNorGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
            circuit.createXnorGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
            circuit.createInputGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
            circuit.createOutputGateAt(centerPos, font, gridDots);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete) {
            circuit.deleteGates();
        }

        for (auto& button : buttons) {
            button.handleEvent(event, window);
            if (button.clicking()) {
                if (button.getCategory() == static_cast<int>(CategoryButton::Cursor)) {
                    
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Editing)) {

                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::And)) {
                    circuit.createAndGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Or)) {
                    circuit.createOrGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Xor)) {
                    circuit.createXorGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Nand)) {
                    circuit.createNandGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Nor)) {
                    circuit.createNorGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Xnor)) {
                    circuit.createXnorGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Input)) {
                    circuit.createInputGateAt(centerPos, font, gridDots);
                }

                else if (button.getCategory() == static_cast<int>(CategoryButton::Output)) {
                    circuit.createOutputGateAt(centerPos, font, gridDots);
                }
            }
        }
    }
}

void Logisim::render()
{
    window.clear(sf::Color::Black);
    window.draw(panelS);
    window.draw(workplaceS);

    for (const auto& dot : gridDots) {
        window.draw(dot);
    }

    for (auto& button : buttons) {
        button.render(window);
    }

    circuit.update();
    circuit.draw(window);

    window.display();
}