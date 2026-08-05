#ifndef PLATFORMER_GAMEROOM_H
#define PLATFORMER_GAMEROOM_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Updatable.h"
#include "Key.h"
#include "Door.h"
#include <unordered_map>
#include <iostream>
#include "Callback.hpp"

// GameRoom class intends be where the logic for each room happens, to avoid clutter on game.cpp
// Each room is a class which inherits this class 
class GameRoom : public Updatable
{
public:
	GameRoom(sf::RenderWindow& window) : window(window) { std::cout << "I Am Being Made\n"; };
	virtual ~GameRoom();
	virtual void init() { }												// To be overriden 
	void update() override;												// To be also overriden (I think it's unlikely) | Should call GameRoom.Update instead of .update
	virtual void onLeftButtonPressedInGame() { }
	virtual void onLeftButtonReleasedInGame() { }

	Callback<std::string> requestPlaySound;

	int GetControlState();
	void inputDragKey();
	void inputReleaseDraggedKey();
	bool inputInteractWithDoor();
	std::string inputMergekeys();
	void inputChangeControls();
	bool tryMergeObjectsIntoOneKey(std::vector<std::string> objects, std::string name);
	std::vector<std::string> getObjectsAtPoint(sf::Vector2f point);

	std::vector<std::reference_wrapper<GameObject>> updateRenderOrder();
	std::unordered_map<std::string, GameObject*> world_objects;			// May update - can never have the same name as another
	std::vector<GameObject*> static_objects;							// Never update

	sf::Sprite background_sprite;
	sf::Texture background_texture;
	bool room_completed = false;										// To check if the room is completed

	sf::RenderWindow& window;

protected:
	Key* dragged_key = nullptr;											// For key dragging	
	int current_control = 1;											// 1 - drag | 2 - combine | 3 - interact
									
};

#endif // !PLATFORMER_GAMEROOM_H
