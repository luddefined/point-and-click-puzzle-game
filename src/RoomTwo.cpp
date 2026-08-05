#include "RoomTwo.h"
#include <set>

RoomTwo::~RoomTwo()
{
}

void RoomTwo::init()
{
	// --- Background setup
	background_texture.loadFromFile("../Data/CustomImages/background.png");
	background_sprite.setTexture(background_texture);
	background_sprite.setScale(
		window.getSize().x / background_sprite.getGlobalBounds().width,
		window.getSize().y / background_sprite.getGlobalBounds().height);

	keyX = std::make_unique<SpriteData>();
	keyY = std::make_unique<SpriteData>();
	keyG = std::make_unique<SpriteData>();
	keyYG = std::make_unique<SpriteData>();
	keyGY = std::make_unique<SpriteData>();
	apple = std::make_unique<SpriteData>();
	potion = std::make_unique<SpriteData>();
	slime = std::make_unique<SpriteData>();
	door_sprite = std::make_unique<SpriteData>();

	keyX->setImage("../Data/CustomImages/keys/key.png");			// Blank key
	keyY->setImage("../Data/CustomImages/keys/keyC.png");			// Yellow key
	keyG->setImage("../Data/CustomImages/keys/keyD.png");			// Green key
	keyYG->setImage("../Data/CustomImages/keys/keyCD.png");			// YG key
	keyGY->setImage("../Data/CustomImages/keys/keyDC.png");			// GY key
	apple->setImage("../Data/CustomImages/apple.png");				// Apple
	potion->setImage("../Data/CustomImages/bottle.png");			// Bottle
	slime->setImage("../Data/CustomImages/slime.png");				// Slime
	door_sprite->setImage("../Data/CustomImages/doors/door3.png");	// GY Door
	keyX->load();
	keyY->load();
	keyG->load();
	keyYG->load();
	keyGY->load();
	apple->load();
	potion->load();
	slime->load();
	door_sprite->load();

	// --- Loading objects
	world_objects.emplace("key", new Key());
	world_objects["key"]->setSpriteData(*keyX);
	world_objects["key"]->layer = 1;
	world_objects["key"]->setSprite();
	world_objects["key"]->getSprite().setPosition(500, 250);
	static_cast<Key*>(world_objects["key"])->merge_mask = "X";

	world_objects.emplace("Y", new Key());
	world_objects["Y"]->setSpriteData(*keyY);
	world_objects["Y"]->layer = 2;
	world_objects["Y"]->setSprite();
	world_objects["Y"]->getSprite().setPosition(400, 400);
	static_cast<Key*>(world_objects["Y"])->key_reference = "Y";
	static_cast<Key*>(world_objects["Y"])->merge_mask = "G";

	world_objects.emplace("apple", new Key());
	world_objects["apple"]->setSpriteData(*apple);
	world_objects["apple"]->layer = 3;
	world_objects["apple"]->setSprite();
	world_objects["apple"]->getSprite().setPosition(100, 300);
	static_cast<Key*>(world_objects["apple"])->merge_mask = "S";

	world_objects.emplace("potion", new Key());
	world_objects["potion"]->setSpriteData(*potion);
	world_objects["potion"]->layer = 4;
	world_objects["potion"]->setSprite();
	world_objects["potion"]->getSprite().setPosition(100, 200);
	static_cast<Key*>(world_objects["potion"])->merge_mask = "S";

	world_objects.emplace("door", new Door());
	world_objects["door"]->layer = 0;
	world_objects["door"]->setSpriteData(*door_sprite);
	world_objects["door"]->setSprite();
	world_objects["door"]->getSprite().setPosition(window.getSize().x / 2 - 100, window.getSize().y - 300);
	static_cast<Door*>(world_objects["door"])->setDoorLock("GY");
}

void RoomTwo::update()
{
	GameRoom::update();
}

void RoomTwo::onLeftButtonPressedInGame()
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
			std::cout << "Merge : " << new_key_name << std::endl;
			// "Process" the new key that comes from this
			// Sort out its sprite and all
			if (!new_key_name.empty()) processMergeLogic(new_key_name);
			break;
		}
	}
}

void RoomTwo::onLeftButtonReleasedInGame()
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

// This is effectively where all the level logic goes
// Runs whenever merging happens
void RoomTwo::processMergeLogic(std::string merged)
{
	// These sets are possible names for when the player tries to merge things
	std::set<std::string> slime_set =	{ "applepotion", "potionapple" };			//    The slime

	std::set<std::string> key_set =		{ "keyapplepotion", "applepotionkey",		// |  Green key
										"potionapplekey", "keypotionapple" };		// |

	std::set<std::string> YG_set =		{ "Ykeyapplepotion", "Yapplepotionkey",		// |  Yellow-green key
										"Ypotionapplekey", "Ykeypotionapple" };		// |

	std::set<std::string> GY_set =		{ "keyapplepotionY", "applepotionkeyY",		// |  Green-yellow key
										"potionapplekeyY", "keypotionappleY" };		// |

	// Making a slime
	if (slime_set.find(merged) != slime_set.end())
	{
		world_objects[merged]->setSpriteData(*slime);
		world_objects[merged]->setSprite();
		static_cast<Key*>(world_objects[merged])->merge_mask = "X";
	}

	// Making a green key
	else if (key_set.find(merged) != key_set.end())
	{
		world_objects[merged]->setSpriteData(*keyG);
		world_objects[merged]->setSprite();
		static_cast<Key*>(world_objects[merged])->merge_mask = "G";
	}

	// Making a yellow-green key
	else if (YG_set.find(merged) != YG_set.end())
	{
		world_objects[merged]->setSpriteData(*keyYG);
		world_objects[merged]->setSprite();
		static_cast<Key*>(world_objects[merged])->key_reference = "YG";
	}

	// Making a green-yellow key (opens door)
	else if (GY_set.find(merged) != GY_set.end())
	{
		world_objects[merged]->setSpriteData(*keyGY);
		world_objects[merged]->setSprite();
		static_cast<Key*>(world_objects[merged])->key_reference = "GY";
	}
}
