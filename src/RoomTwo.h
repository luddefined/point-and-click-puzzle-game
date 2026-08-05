#ifndef ROOMTWO_H
#define ROOMTWO_H

#include "GameRoom.h"
#include "Key.h"
#include "SpriteData.h"
#include <SFML/Graphics.hpp>

class RoomTwo : public GameRoom
{
public:
	using GameRoom::GameRoom;

	~RoomTwo();

	void init() override;
	void update() override;

	void onLeftButtonPressedInGame() override;
	void onLeftButtonReleasedInGame() override;

	void processMergeLogic(std::string merged);

private:
	// Sprites
	std::unique_ptr<SpriteData> keyX, keyY, keyG, keyYG, keyGY, apple, potion, slime, door_sprite;
};

#endif