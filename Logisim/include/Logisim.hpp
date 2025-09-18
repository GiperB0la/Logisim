#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>

#include "CircuitManager.hpp"
#include "../Widgets/include/Button.hpp"

#ifdef _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-audio.lib")
#endif

enum class CategoryButton {
    Cursor,
    Editing,
    And,
    Or,
    Xor,
    Nand,
    Nor,
    Xnor,
    Input,
    Output
};

class Logisim
{
public:
    Logisim();

public:
    void run();

private:
    void init();
    void createButtons();
    void processEvents();
    void render();

private:
    sf::ContextSettings ctx;
    sf::RenderWindow window;
    std::vector<sf::CircleShape> gridDots;
    sf::Font font;

    sf::Texture panelT, workplaceT;
    sf::Sprite panelS, workplaceS;

    CircuitManager circuit;

    std::vector<Button> buttons;

    sf::Vector2f workplacePosition;
    sf::Vector2f workplaceSize;
};