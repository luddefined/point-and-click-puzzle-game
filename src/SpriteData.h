#ifndef SPRITEDATA_H
#define SPRITEDATA_H

#include "SFML/Graphics.hpp"
#include <iostream>

// This exists to avoid sprites needing their own unique texture which constantly loads itself

// Container for data used for a GameObject's sprite
struct SpriteData
{
public:
	SpriteData() { };
	~SpriteData() { std::cout << "Deleting\n"; }

	sf::Texture texture;								// Texture for the sprite
	bool texture_loaded = false;						// Whether texture has loaded

	void setImage(std::string file);
	bool load();
	bool load(std::string file);

private:
	std::string source_image;							// Image for the texture (for the sprite)
};

#endif
