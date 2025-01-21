#include "./assets/dependencies/stb_image.h"
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "./assets/scripts/utils.h"
#include "./assets/scripts/GUI/Displayable.h"
#include "./assets/scripts/GUI/Layering.h"
#include "./assets/scripts/Control/Level.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
	//Create variables to store the window, its surface, and the renderer
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;

	//Ensure that SDL was properly initialized
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialize" << endl;
		cout << SDL_GetError() << endl;
		return 1;
	}

	//Store the window, the window's surface, and the window's renderer
	window = util::makeWindow("Chess 2", util::SCREEN_WIDTH, util::SCREEN_HEIGHT);
	renderer = SDL_CreateRenderer(window, -1, 0);
	surface = SDL_GetWindowSurface(window);

	//ctrl::Level* currlvl = new ctrl::mainMenu(renderer);
	ctrl::Level* currlvl = new ctrl::std_Chess(renderer);

	//Create the main game loop
	bool RUNNING = true;
	while (RUNNING) {
		//Iterate through the list of events, handling each independently
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			//Handle key presses
			if (event.type == SDL_KEYDOWN) {
				SDL_Keycode key = event.key.keysym.sym;

				//Quit if the 'q' key is pressed
				if (key == SDLK_q) RUNNING = false;

				if (key == SDLK_u) currlvl->undoAction();
			}

			//Handle the window exit
			else if (event.type == SDL_QUIT) RUNNING = false;

			//Handle mouse events
			else if (event.type == SDL_MOUSEBUTTONDOWN)
				currlvl->handleClick();
		}

		currlvl->update();

		//Fill the background of the window
		SDL_SetRenderDrawColor(renderer, 0xED, 0xDF, 0xF7, 0xFF);
		SDL_RenderClear(renderer);

		//Render each item in the asset list onto the render surface
		currlvl->render();

		//Render all items within the renderer to the screen
		SDL_RenderPresent(renderer);

		if (currlvl->getState() == util::ANONYMOUS) continue;
		
		std::vector<string> newstate = util::unpackState(currlvl->getState());
		
		if (newstate[0] == "goto") {
			if (newstate[1] == "gameselect") {
				delete currlvl;
				currlvl = new ctrl::gameSelect(renderer);
			}

			if (newstate[1] == "std_chess") {
				delete currlvl;
				currlvl = new ctrl::std_Chess(renderer);
			}
		}
	}

	delete currlvl;

	//Clean up the memory to prevent leaks
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return 0;
}