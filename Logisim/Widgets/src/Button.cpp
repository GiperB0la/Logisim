#include "../include/Button.hpp"


Button::Button(
	const size_t category,
	const sf::Vector2f& position,
	const std::string& normal_condition,
	const std::string& hovered_condition,
	const std::string& pressed_condition,
	const std::wstring& text,
	const sf::Font& font,
	size_t size_text,
	const sf::Color color_text_normal,
	const sf::Color color_text_hovered,
	const sf::Color color_text_pressed,
	const sf::Vector2f& position_text)
	: category(category),
	position(position),
	text(text, font, size_text),
	size_text(size_text),
	color_text_normal(color_text_normal),
	color_text_hovered(color_text_hovered),
	color_text_pressed(color_text_pressed),
	position_text(position_text)
{
	normal_condition_texture.loadFromFile(normal_condition);
	hovered_condition_texture.loadFromFile(hovered_condition);
	pressed_condition_texture.loadFromFile(pressed_condition);

	button.setPosition(position);
	button.setTexture(normal_condition_texture);

	this->text.setFillColor(color_text_normal);

	sf::FloatRect buttonBounds = button.getGlobalBounds();
	sf::FloatRect textBounds = this->text.getLocalBounds();

	if (position_text != sf::Vector2f(0.f, 0.f)) {
		this->text.setPosition(position + position_text);
	}
	else {
		this->text.setPosition(
			buttonBounds.left + (buttonBounds.width - textBounds.width) / 2 - textBounds.left,
			buttonBounds.top + (buttonBounds.height - textBounds.height) / 2 - textBounds.top
		);
	}

	state = State::Normal;
}

void Button::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

	bool isMouseOver = button.getGlobalBounds().contains(worldPos);
	isCursor = isMouseOver;

	if (event.type == sf::Event::MouseMoved) {
		if (isMouseOver && state != State::Pressed) {
			state = State::Hovered;
		}
		else if (!isMouseOver) {
			state = State::Normal;
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (isMouseOver) {
			state = State::Pressed;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		if (isMouseOver && state == State::Pressed) {
			click = true;
		}
		state = isMouseOver ? State::Hovered : State::Normal;
	}
}

void Button::render(sf::RenderWindow& window)
{
	switch (state) {
	case State::Normal:
		button.setTexture(normal_condition_texture);
		text.setFillColor(color_text_normal);
		break;
	case State::Hovered:
		button.setTexture(hovered_condition_texture);
		text.setFillColor(color_text_hovered);
		break;
	case State::Pressed:
		button.setTexture(pressed_condition_texture);
		text.setFillColor(color_text_pressed);
		break;
	}

	window.draw(button);
	window.draw(text);
}

bool Button::clicking()
{
	if (click) {
		click = false;
		return true;
	}
	return false;
}

int Button::getCategory()
{
	return category;
}

sf::Vector2f Button::getPosition()
{
	return position;
}

void Button::setPosition(const sf::Vector2f& position)
{
	this->position = position;
	button.setPosition(position);
}

void Button::setPosition(float positionX, float positionY)
{
	this->position = sf::Vector2f(positionX, positionY);
	button.setPosition(position);
}

bool Button::cursor()
{
	return isCursor;
}

void Button::setTexture(int state, const std::string& newTexture)
{
	switch (state)
	{
	case 0:
		normal_condition_texture.loadFromFile(newTexture);
		break;
	case 1:
		hovered_condition_texture.loadFromFile(newTexture);
		break;
	case 2:
		pressed_condition_texture.loadFromFile(newTexture);
		break;
	default:
		break;
	}
}

void Button::updateCursor(sf::RenderWindow& window)
{
	sf::Cursor cursor;

	if (isCursor) {
		if (cursor.loadFromSystem(sf::Cursor::Hand)) {
			window.setMouseCursor(cursor);
		}
	}
	else {
		if (cursor.loadFromSystem(sf::Cursor::Arrow)) {
			window.setMouseCursor(cursor);
		}
	}
}

void Button::setScale(float scaleX, float scaleY)
{
	button.setScale(scaleX, scaleY);

	text.setScale(scaleX, scaleY);

	sf::FloatRect buttonBounds = button.getGlobalBounds();
	sf::FloatRect textBounds = text.getLocalBounds();

	text.setPosition(
		buttonBounds.left + (buttonBounds.width - textBounds.width * scaleX) / 2 - textBounds.left * scaleX,
		buttonBounds.top + (buttonBounds.height - textBounds.height * scaleY) / 2 - textBounds.top * scaleY
	);
}