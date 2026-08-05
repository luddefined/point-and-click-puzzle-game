
#include "Game.h"
#include "GameObject.h"
#include "Player.h"
#include "GameRoom.h"
#include "RoomOne.h"
#include "RoomTwo.h"
#include <iostream>

/* Moving this to .h 
Game::Game(sf::RenderWindow& game_window) 
{
	;
}
*/

Game::~Game()
{

}

// Game state enum
enum class GameState
{
	MENU = 0,   // Initial state
	GAME = 1,   // State while in level
};
GameState game_state;

bool Game::init()
{
	// current_room = room1;
	game_state = GameState::GAME;
	setRoom();

	// Text initalisation
	font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf");

	control_text.setFont(font);
	control_text.setFillColor(sf::Color::White);
	control_text.setCharacterSize(28);
	setControlTextString(current_room->GetControlState());
	control_text.setPosition(
		window.getSize().x * 0.01,
		window.getSize().y * 0.95 - control_text.getGlobalBounds().height);

	result_text.setFont(font);
	result_text.setFillColor(sf::Color::White);
	result_text.setCharacterSize(48);

	// Player init
	player_sprite_data.setImage("../Data/CustomImages/dude.png");
	player_sprite_data.load();
	player.init(player_sprite_data)->getSprite().setScale(1.5, 1.5);
	player.setPosition(
		window.getSize().x * 0.9,
		window.getSize().y - player.getPlayerGameObject()->getSprite().getGlobalBounds().height);
		
	// Sounds
	sound_manager.addSound("merge", "../Data/Audio/merge.wav");
	sound_manager.addSound("door", "../Data/Audio/door.wav");
	sound_manager.addSound("switch", "../Data/Audio/Switch.wav");

	return true;
}

void Game::update(float dt)
{
	// std::cout << "--- NEW UPDATE CYCLE ---" << std::endl;
	current_room->Update();

	if (current_room->room_completed)
	{
		// Increment room index and change the room
		std::cout << "Room completed\n";
		if (++current_room_index < 3)
		{
			delete current_room;
			setRoom();
		}
		else
		{
			control_text.setCharacterSize(48);
			control_text.setString("well done!!\nyou win");
			control_text.setPosition(
				window.getSize().x / 2 - control_text.getGlobalBounds().width / 2,
				window.getSize().y / 2 - control_text.getGlobalBounds().height / 2);
			player.getPlayerGameObject()->getSprite().setPosition(
				window.getSize().x / 2 - player.getPlayerGameObject()->getSprite().getGlobalBounds().width / 2,
				600);
		}
	}
}

void Game::render()
{
	std::vector<std::reference_wrapper<GameObject>> render_order;
	render_order = current_room->updateRenderOrder();

	// Draw objects - BG first, then objects, then player, then text
	window.draw(current_room->background_sprite);
	for (const auto& object : render_order)
	{
		if (current_room->room_completed) break;// Do not need to draw room
		if (!object.get().enabled) continue;	// Object does not need to be drawn

		if (auto* sprite = &object.get().getSprite())
		{
			window.draw(*sprite); // holy moly
		}
	}
	window.draw(player.getPlayerGameObject()->getSprite());
	window.draw(control_text);
}

void Game::mouseButtonPressed(sf::Event event)
{
	switch (event.mouseButton.button)
	{
		// Left mouse button pressed
		case (sf::Mouse::Button::Left):
		{
			switch (game_state)
			{
				case (GameState::MENU):
				{
					// In Menu
					break;
				}
				case (GameState::GAME):
				{
					// In game
					current_room->onLeftButtonPressedInGame();
					break;
				}
			}
			break;
		}

		// Right mouse button pressed
		case (sf::Mouse::Button::Right):
		{
			switch (game_state)
			{
				case (GameState::MENU):
				{
					// In Menu
					break;
				}
				case (GameState::GAME):
				{
					// In game
					current_room->inputChangeControls();
					setControlTextString(current_room->GetControlState());
					sound_manager.play("switch", sound_manager.getRandomPitch(sound_manager.BASE_PITCH, sound_manager.PITCH_RANGE));
					break;
				}
			}
			break;
		}
	}
}

void Game::mouseButtonReleased(sf::Event event)
{
	switch (event.mouseButton.button)
	{
		// Left mouse button released
		case (sf::Mouse::Left):
		{
			switch (game_state)
			{
				case (GameState::MENU):
				{
					// In Menu
				}
				case (GameState::GAME):
				{
					// In game
					current_room->onLeftButtonReleasedInGame();
				}
			}
		}

		// Right mouse button released
		case (sf::Mouse::Right):
		{
			switch (game_state)
			{
				case (GameState::MENU):
				{
					// In Menu
				}
				case (GameState::GAME):
				{
					// In game
				}
			}
		}
	}
}

void Game::keyReleased(sf::Event event)
{
	// If not in game then do nothing (keyboard input not needed)
	if (!(game_state == GameState::GAME)) return;

	switch (event.key.code)
	{
		case (sf::Keyboard::R):
		{
			// Reset the room
			if (current_room->room_completed) break;
			resetRoom();
			break;
		}
	}
}

void Game::resetRoom()
{
	delete current_room;
	
	setRoom();
	setControlTextString(current_room->GetControlState());
}

void Game::setRoom()
{

	if (current_room_index == 1)
	{
		current_room = new RoomOne(window);
	}
	else if (current_room_index == 2)
	{
		current_room = new RoomTwo(window);
	}
	current_room->init();
	current_room->requestPlaySound.add(this, playSoundCallback);

	// Set this specific text to be the right text
	setControlTextString(current_room->GetControlState());
}

void Game::setControlTextString(int num)
{
	if (num == 1)	   control_text_string_state = "Dragging";
	else if (num == 2) control_text_string_state = "Interacting";
	else if (num == 3) control_text_string_state = "Merging";
	control_text.setString(control_text_string + control_text_string_state);
}

void Game::playSoundCallback(std::string name)
{
	sound_manager.play(name, sound_manager.getRandomPitch(sound_manager.BASE_PITCH, sound_manager.PITCH_RANGE));
}
