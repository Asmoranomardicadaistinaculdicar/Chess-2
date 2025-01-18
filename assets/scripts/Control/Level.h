#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>

#include "../GUI/Displayable.h"
#include "../GUI/Layering.h"

//Define the Control namespace
namespace ctrl {

	class Level {
	protected:
		SDL_Renderer* renderer = nullptr;
		GUI::PegBar assets;
		std::string state;
		int mx, my;

	public:
		virtual void handleClick() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		std::string getState() { return this->state; }
	};

	class mainMenu : public Level {
	public:
		mainMenu(SDL_Renderer* renderer) {
			//Store the current renderer
			this->renderer = renderer;

			//Store the initial mouse position
			this->mx = util::SCREEN_WIDTH / 2;
			this->my = util::SCREEN_HEIGHT / 2;
			SDL_GetMouseState(&(this->mx), &(this->my));

			//Initialize the state to an empty value
			this->state = util::ANONYMOUS;

			//Insert the icon into the asset structure
			this->assets.insertIntoLayer(
				"icon",
				new GUI::Displayable(
					renderer, "./assets/texture/screens/main-menu.png",
					{ 0, 0, util::SCREEN_WIDTH, util::SCREEN_HEIGHT }),
				0);

			GUI::Button* playbutton =
				new GUI::Button({
					util::SCREEN_WIDTH / 2 - (267 / 2),
					util::SCREEN_HEIGHT / 3 * 2,
					267, 92
					},
					"play");
			playbutton->givePose(
				"hover",
				"./assets/texture/buttons/play/hover.png",
				this->renderer
				);
			playbutton->givePose(
				"neutral",
				"./assets/texture/buttons/play/neutral.png",
				this->renderer
				);
			this->assets.insertIntoLayer(
				"play_button",
				playbutton,
				0
				);
		}
		~mainMenu() {}

		void handleClick();
		void update();
		void render();
	};

	class gameSelect : public Level {
	public:
		gameSelect(SDL_Renderer* renderer) {
			//Store the current renderer
			this->renderer = renderer;

			//Store the current mouse position
			this->mx = util::SCREEN_WIDTH / 2;
			this->my = util::SCREEN_HEIGHT / 2;
			SDL_GetMouseState(&this->mx, &this->my);

			//Initialize the state to nothing
			this->state = util::ANONYMOUS;

			GUI::Button* std_chess =
				new GUI::Button({
				util::SCREEN_WIDTH / 2 - (216 / 2),
				util::SCREEN_HEIGHT / 2 - (216 / 2),
				216, 216 },
				"std_chess"
				);
			std_chess->givePose(
				"neutral",
				"./assets/texture/buttons/std_chess/neutral.png",
				this->renderer
				);
			std_chess->givePose(
				"hover",
				"./assets/texture/buttons/std_Chess/hover.png",
				this->renderer
				);
			this->assets.insertIntoLayer("std_chess", std_chess, 0);
		}
		~gameSelect() {};

		void handleClick();
		void update();
		void render();
	};
}

#endif