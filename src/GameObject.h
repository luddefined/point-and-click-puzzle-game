#ifndef PLATFORMER_GAMEOBJECT_H
#define PLATFORMER_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "Updatable.h"
#include "SpriteData.h"
#include <memory>

// Game objects include items and doors
class GameObject : public Updatable
{
public:
	GameObject();
	GameObject(SpriteData& s);								
	GameObject(int layer);									// Quick assign for the layer
	virtual ~GameObject();


	virtual void update() override;
	sf::Sprite& getSprite() const;
	void setSpriteData(SpriteData& data);
	void setSprite();
	void setSprite(std::string file);
	bool enabled = true;
	int layer = 0;											// Affects how this is drawn - higher means above other sprites

protected:
	std::unique_ptr<sf::Sprite> _sprite;					// I've seen in other programs this sprite just be raw pointer

private:
	SpriteData* sprite_data;

};															


#endif