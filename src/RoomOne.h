#ifndef ROOMONE_H
#define ROOMONE_H

#include "GameRoom.h"
#include "Key.h"
#include "SpriteData.h"
#include <SFML/Graphics.hpp>

class RoomOne : public GameRoom
{
public:
	using GameRoom::GameRoom;
	~RoomOne();

	void init() override;
	void update() override;

	void onLeftButtonPressedInGame() override;
	void onLeftButtonReleasedInGame() override;

private:
	// Sprites
	std::unique_ptr<SpriteData> keyA, keyB, keyAB, keyBA, door_sprite;

};

#endif
