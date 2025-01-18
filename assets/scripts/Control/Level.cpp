#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>

#include "../GUI/Displayable.h"
#include "../GUI/Layering.h"
#include "./Level.h"

using GUI::Displayable;
using GUI::Button;
using std::string;

//Define the Control namespace
namespace ctrl {
	void mainMenu::handleClick() {
		SDL_GetMouseState(
			&(this->mx),
			&(this->my)
		);

		Button* playbutton = (Button*)(this->assets.getAssetByKey("play_button"));
		if (playbutton->collidepoint(this->mx, this->my)) {
			this->state = "goto gameselect";
		}

		return;
	}

	void mainMenu::update() {
		SDL_GetMouseState(
			&(this->mx),
			&(this->my)
		);

		Button* playbutton = (Button*)(this->assets.getAssetByKey("play_button"));
		if (playbutton->collidepoint(this->mx, this->my)) {
			playbutton->setPose("hover");
		} else {
			playbutton->setPose("neutral");
		}

		return;
	}

	void mainMenu::render() {
		if (!this->assets.render(this->renderer))
			std::cout << this->assets.getError() << std::endl;
		return;
	}


	void gameSelect::handleClick() {
		SDL_GetMouseState(&this->mx, &this->my);

		for (string key : this->assets.getKeys()) {
			Button* btn = (Button*)(this->assets.getAssetByKey(key));
			if (btn->collidepoint(this->mx, this->my)) {
				if (btn->getFlag() == util::ANONYMOUS) {
					std::cout << "gameSelect.handleClick(): Button flag not set";
					std::cout << std::endl;
					continue;
				}
				this->state = "goto " + btn->getFlag();
			}
		}

		return;
	}

	void gameSelect::update() {
		SDL_GetMouseState(&this->mx, &this->my);

		for (string key : this->assets.getKeys()) {
			Button* btn = (Button*)(this->assets.getAssetByKey(key));
			if (btn->collidepoint(this->mx, this->my)) {
				btn->setPose("hover");
			} else {
				btn->setPose("neutral");
			}
		}

		return;
	}

	void gameSelect::render() {
		if (!this->assets.render(this->renderer))
			std::cout << this->assets.getError() << std::endl;
		return;
	}
}