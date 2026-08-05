#ifndef UPDATABLE_H
#define UPDATABLE_H

#include "SFML/Graphics.hpp"

// Class for adding draggable behaviour to game objects
// Should be able to do all the dragging behaviour itself
class Draggable
{
public:
	Draggable();
	sf::RenderWindow* drag_window;
	sf::Sprite* sprite;
	void setDragged(bool state);
	void updateSprite();


private:
	bool dragged = false;
	void updateOffset();
	sf::Vector2f getClickPos();
	sf::Vector2f drag_offset;
};

#endif
