#include "RoomOne.h"
#include "Key.h"
#include "GameObject.h"
#include "Door.h"
#include <iostream>

RoomOne::~RoomOne()
{
	std::cout << "removing\n";
}

void RoomOne::init()
{
	// --- Background setup
	background_texture.loadFromFile("../Data/CustomImages/background.png");
	background_sprite.setTexture(background_texture);
	background_sprite.setScale(
		window.getSize().x / background_sprite.getGlobalBounds().width,
		window.getSize().y / background_sprite.getGlobalBounds().height);

	// --- Sprite setup
	keyA = std::make_unique<SpriteData>();
	keyB = std::make_unique<SpriteData>();
	keyAB = std::make_unique<SpriteData>();
	keyBA = std::make_unique<SpriteData>();
	door_sprite = std::make_unique<SpriteData>();

	keyA->setImage("../Data/CustomImages/keys/keyA.png");
	keyB->setImage("../Data/CustomImages/keys/keyB.png");
	keyAB->setImage("../Data/CustomImages/keys/keyAB.png");
	keyBA->setImage("../Data/CustomImages/keys/keyBA.png");
	door_sprite->setImage("../Data/CustomImages/doors/door2.png");
	keyA->load();
	keyB->load();
	keyAB->load();
	keyBA->load();
	door_sprite->load();

	// --- Loading objects
	world_objects.emplace("A", new Key());
	world_objects["A"]->setSpriteData(*keyA);
	world_objects["A"]->layer = 1;
	world_objects["A"]->setSprite();
	world_objects["A"]->getSprite().setPosition(200, 500);
	static_cast<Key*>(world_objects["A"])->key_reference = "A";

	world_objects.emplace("B", new Key());
	world_objects["B"]->setSpriteData(*keyB);
	world_objects["B"]->layer = 2;
	world_objects["B"]->setSprite();
	world_objects["B"]->getSprite().setPosition(400, 200);
	static_cast<Key*>(world_objects["B"])->key_reference = "B";

	world_objects.emplace("door", new Door());
	world_objects["door"]->layer = 0;
	world_objects["door"]->setSpriteData(*door_sprite);
	world_objects["door"]->setSprite();
	world_objects["door"]->getSprite().setPosition(window.getSize().x/2 - 100, window.getSize().y - 300);
	static_cast<Door*>(world_objects["door"])->setDoorLock("AB");

	static_objects.push_back(new Key());
	
	
}

// Code here runs after all world_objects' update has run
void RoomOne::update()
{
	GameRoom::update();
	// std::cout << "Room One update\n";
}

void RoomOne::onLeftButtonPressedInGame()
{
	switch (current_control)
	{
		case (1):
		{
			/// --- START DRAGGING A KEY ---
			inputDragKey();
			break;
		}
		case (2):
		{
			/// --- INTERACT WITH DOOR ---
			bool interaction = inputInteractWithDoor();
			std::cout << "Interaction : " << interaction << std::endl;
			room_completed = interaction;
			break;
		}
		case (3):
		{
			/// --- MERGE KEYS ---
			std::string new_key_name = inputMergekeys();
			// Set the correct sprite
			// In this case its simple to check the new name with the two possible types of names
			if (new_key_name == "AB")
			{
				world_objects[new_key_name]->setSpriteData(*keyAB);
				world_objects[new_key_name]->layer = 3;
				world_objects[new_key_name]->setSprite();
			}
			else if (new_key_name == "BA")
			{
				world_objects[new_key_name]->setSpriteData(*keyBA);
				world_objects[new_key_name]->layer = 3;
				world_objects[new_key_name]->setSprite();
			}
			std::cout << "Merge : " << new_key_name << std::endl;
			// If new key name exists then handle updating the sprite and layer and all
			break;
		}
	}
}

void RoomOne::onLeftButtonReleasedInGame()
{
	switch (current_control)
	{
		case(1):
		{
			/// --- RELEASE DRAGGING KEY ---
			inputReleaseDraggedKey();
			break;
		}
	}
}
