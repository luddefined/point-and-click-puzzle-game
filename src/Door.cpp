#include "Door.h"

Door::Door()
{

}

Door::~Door()
{

}

void Door::setDoorLock(std::string str)
{
	door_lock = str;
}

bool Door::isOpen() const
{
	return opened;
}

// Attempt to open the door and return if successful or not
bool Door::tryOpen(Key& door_key)
{
	if (opened) return true;

	// Only open door if the door_key's key_reference is the exact same as the door's door_lock
	if (door_key.key_reference == door_lock)
	{
		opened = true;
		return true;
	}

	// Door is not open
	return false;
}

// Forcibly lock the door
void Door::forceLock()
{
	opened = false;
}

