#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	enabled = true;
	_sprite = std::make_unique<sf::Sprite>();
}

GameObject::GameObject(SpriteData& s) : GameObject()
{
	sprite_data = &s;
	sprite_data->load();
}

GameObject::GameObject(int l) : GameObject()
{
	layer = l;
}

GameObject::~GameObject()
{
	sprite_data = nullptr;
}

void GameObject::update()
{
	Updatable::update();
}

sf::Sprite& GameObject::getSprite() const
{
	return *_sprite;
}

// Load the object's texture and assign it to the sprite
void GameObject::setSprite(std::string file)
{
	// If no sprite data exists, do nothing
	if (!sprite_data) return;

	// Load sprite texture if the sprite isn't loaded
	// Set game object sprite texture
	if (!sprite_data->texture_loaded) sprite_data->load(file);
	_sprite->setTexture(sprite_data->texture);

}

void GameObject::setSpriteData(SpriteData& data)
{
	sprite_data = &data;
	sprite_data->load();
}

// Sets the sprite of the object so long as its texture has already been loaded
void GameObject::setSprite()
{
	if (!sprite_data) return;
	if (sprite_data->texture_loaded)
	{
		_sprite->setTexture(sprite_data->texture);
	}
}

