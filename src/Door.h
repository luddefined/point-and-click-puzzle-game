#ifndef DOOR_H
#define DOOR_H

#include "GameObject.h"
#include "Key.h"
#include "SFML/Graphics.hpp"

class Door : public GameObject
{
public:
	using GameObject::GameObject;
	Door();
	~Door();

	// std::string getDoorLock() const { return door_lock; };	Not sure if needed
	void setDoorLock(std::string str);
	bool isOpen() const;
	bool tryOpen(Key& door_key);
	void forceLock();

private:
	std::string door_lock;
	bool opened = false;

};

#endif
