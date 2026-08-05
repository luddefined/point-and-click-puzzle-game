#include "Player.h"

Player::Player()
{
	player_object = std::make_unique<GameObject>();
}

GameObject* Player::init(SpriteData& sprite_data)
{
	if (!player_object) return nullptr;

	player_object->setSpriteData(sprite_data);
	player_object->setSprite();
	return player_object.get();
}

void Player::setPosition(float x, float y)
{
	player_object->getSprite().setPosition(x, y);
}

