#include "./assets/dependencies/stb_image.h"
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "./assets/scripts/utils.h"
#include "./assets/scripts/GUI/Displayable.h"

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

	//Create a list of displayables to render
	std::vector<GUI::Displayable> dispVec;

	//Add the main menu graphic to the display list
	GUI::Displayable icon = GUI::Displayable(
			renderer, "./assets/texture/screens/main-menu.png",
			{ 0, 0, util::SCREEN_WIDTH, util::SCREEN_HEIGHT }
		);
	dispVec.push_back(icon);

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
			}

			//Handle the window exit
			else if (event.type == SDL_QUIT) RUNNING = false;

			//Handle mouse events
			else if (event.type == SDL_MOUSEBUTTONDOWN)
				cout << "You clicked the mouse" << endl;
		}
		
		//Fill the background of the window
		SDL_SetRenderDrawColor(renderer, 0xED, 0xDF, 0xF7, 0xFF);
		SDL_RenderClear(renderer);

		//Render each item in the displayable vector onto the render surface
		for (int x = 0; x < dispVec.size(); x++) {
			if (!dispVec[x].render(renderer))
				cout << dispVec[x].getError() << endl;
				cout << "Boi we out here not rendering" << endl;
		}

		//icon.render(renderer);

		//Render all items within the renderer to the screen
		SDL_RenderPresent(renderer);
	}

	//Clean up the memory to prevent leaks
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return 0;
}