#include "Key.h"
#include <iostream>

Key::Key() : GameObject()
{
	draggable = new Draggable();
	draggable->sprite = _sprite.get();
}

Key::Key(SpriteData& s) : GameObject(s)
{
	draggable = new Draggable();
	draggable->sprite = _sprite.get();
}

Key::~Key()
{
	delete draggable;
	// GameObject::~GameObject();
}

void Key::update()
{
	// std::cout << "Key update\n";
	draggable->updateSprite();
}
