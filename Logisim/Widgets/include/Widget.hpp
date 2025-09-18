#pragma once
#include <SFML/Graphics.hpp>


class Widget
{
public:
	virtual ~Widget() = default;
	virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual sf::Vector2f getPosition() = 0;
	virtual void setPosition(const sf::Vector2f& position) = 0;
};