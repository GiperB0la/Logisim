#pragma once
#include <SFML/Graphics.hpp>
#include "Widget.hpp"


class Button : public Widget
{
public:
	Button() = default;
	Button(
		const size_t category,
		const sf::Vector2f& position,
		const std::string& normal_condition,
		const std::string& hovered_condition,
		const std::string& pressed_condition,
		const std::wstring& text,
		const sf::Font& font,
		size_t size_text = 14,
		const sf::Color color_text_normal = sf::Color::White,
		const sf::Color color_text_hovered = sf::Color::White,
		const sf::Color color_text_pressed = sf::Color::White,
		const sf::Vector2f& position_text = sf::Vector2f());
	~Button() = default;

public:
	void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
	void render(sf::RenderWindow& window) override;
	virtual bool clicking();
	virtual int getCategory();
	sf::Vector2f getPosition() override;
	void setPosition(const sf::Vector2f& position) override;
	void setPosition(float positionX, float positionY);
	bool cursor();
	void setTexture(int state, const std::string& newTexture);
	void updateCursor(sf::RenderWindow& window);
	void setScale(float scaleX, float scaleY);

protected:
	sf::Sprite button;
	size_t category;
	sf::Vector2f position;
	sf::Texture normal_condition_texture;
	sf::Texture hovered_condition_texture;
	sf::Texture pressed_condition_texture;

protected:
	sf::Text text;
	size_t size_text;
	sf::Vector2f position_text;
	sf::Color color_text_normal;
	sf::Color color_text_hovered;
	sf::Color color_text_pressed;

protected:
	enum class State { Normal, Hovered, Pressed };
	State state;

protected:
	bool click = false;
	bool isCursor = false;
};