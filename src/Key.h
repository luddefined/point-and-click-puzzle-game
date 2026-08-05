#ifndef KEY_H
#define KEY_H

#include "GameObject.h"
#include "Draggable.h"
#include "SpriteData.h"
#include "SFML/Graphics.hpp"

class Key : public GameObject
{
public:
	Key();
	Key(SpriteData& s);
	Key(std::string value) : key_reference(value) { draggable = new Draggable() ; draggable->sprite = _sprite.get(); };
	~Key();
	Draggable* draggable = nullptr;
	void update() override;

	// Used to determine what key is used
	std::string key_reference;

	// Used to determine what this object can be merged with
	std::string merge_mask;
};


#endif