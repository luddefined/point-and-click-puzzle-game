#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"


// Just a container for a sprite really
// Could have done this in a struct! oh well...
class Player
{
public:
	Player();
	GameObject* init(SpriteData& sprite_data);

	GameObject* getPlayerGameObject() const { return player_object.get(); }
	void setPosition(float x, float y);
	
private:
	std::unique_ptr<GameObject> player_object;
};

#endif
