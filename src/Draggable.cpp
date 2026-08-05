#include "Draggable.h"
#include "SFML/Graphics.hpp"

void Draggable::updateSprite()
{
	if (!dragged) { return; }	// Don't update if not dragging

	if (sprite != nullptr)
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition(*drag_window);
		sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);

		sf::Vector2f drag_position = mouse_positionf - drag_offset;
		sprite->setPosition(drag_position.x, drag_position.y);
	}
}

sf::Vector2f Draggable::getClickPos()
{
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*drag_window));
}

Draggable::Draggable()
{
	drag_window = nullptr;
	sprite = nullptr;
}

void Draggable::setDragged(bool state)
{
	if (state == true)
	{
		updateOffset();
		dragged = true;
	}
	else if (state == false)
	{
		dragged = false;
	}
}

void Draggable::updateOffset()
{
	drag_offset = getClickPos() - sprite->getPosition();	// lol
}
