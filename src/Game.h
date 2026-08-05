
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include "GameRoom.h"
#include "RoomOne.h"
#include "SpriteData.h"
#include "Player.h"
#include "SoundManager.h"
#include <memory>
#include <string>

class Game
{
	public:
		Game(sf::RenderWindow& window) : window(window) { srand(time(NULL)); }
		~Game();
		bool init();
		void update(float dt);
		void render();
		void mouseButtonPressed(sf::Event event);
		void mouseButtonReleased(sf::Event event);
		void keyReleased(sf::Event event);				// Key press not necessarily needed
		void resetRoom();
		void setRoom();
		void setControlTextString(int num);
		void playSoundCallback(std::string name);
		int current_room_index = 1;
		GameRoom* current_room = nullptr;	
		
		sf::Font font;
		sf::Text control_text;
		sf::Text result_text;

		std::string control_text_string = "[R] to reset\nRight click to change mode\nCurrent mode: ";
		std::string control_text_string_state = "";

		Player player;
		SpriteData player_sprite_data;
		SoundManager sound_manager;

	private:
		sf::RenderWindow& window;
		// RoomOne room_one;

};

#endif // PLATFORMER_GAME_H
