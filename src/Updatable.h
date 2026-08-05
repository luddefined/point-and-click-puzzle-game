#ifndef PLATFORMER_UPDATABLE_H
#define PLATFORMER_UPDATABLE_H

#include <SFML/Graphics.hpp>

// Abstract class for things that are updatable (has an update)
class Updatable
{
public:
	void Update() 
	{
		update();
	}

protected:
	virtual void update() { };
};

#endif