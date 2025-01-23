#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include "../dependencies/stb_image.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using std::cout;
using std::endl;

//Define the utility namespace
namespace util {

	//Define global variables for screen width and height
	static int SCREEN_WIDTH = 1080;
	static int SCREEN_HEIGHT = 720;

	static std::string ANONYMOUS = "_";

	static bool debug = 1;

	enum disp_t {
		DISP_BASIC,
		DISP_BUTTON
	};

	enum piece_t {
		PAWN = 1,
		ROOK = 3,
		KNIGHT = 4,
		BISHOP = 5,
		QUEEN = 9,
		KING = 200
	};

	enum color_t {
		BLACK,
		WHITE
	};

	enum cmd_t {
		MOVE,
		TAKE
	};

	static std::map<piece_t, const char*> bpaths = {
			{PAWN, "./assets/texture/pieces/b_pawn.png"},
			{ROOK, "./assets/texture/pieces/b_rook.png"},
			{KNIGHT, "./assets/texture/pieces/b_knight.png"},
			{BISHOP, "./assets/texture/pieces/b_bishop.png"},
			{QUEEN, "./assets/texture/pieces/b_queen.png"},
			{KING, "./assets/texture/pieces/b_king.png"}
		};

	static std::map<piece_t, const char*> wpaths = {
			{PAWN, "./assets/texture/pieces/w_pawn.png"},
			{ROOK, "./assets/texture/pieces/w_rook.png"},
			{KNIGHT, "./assets/texture/pieces/w_knight.png"},
			{BISHOP, "./assets/texture/pieces/w_bishop.png"},
			{QUEEN, "./assets/texture/pieces/w_queen.png"},
			{KING, "./assets/texture/pieces/w_king.png"}
		};

	/* Initializes an SDL_Window that can be displayed for the player
	* 
	* Preconditions:
	* - SDL Must be initialized
	* 
	* Postcondition:
	* - A window is created with the provided size values, centered on screen
	* 
	* Params:
	* - caption - the name displayed for the game's window
	* - width - the width of the display window
	* - height - the height of the display indow
	* 
	* Returns a pointer to the Window provided
	*/
	inline SDL_Window* makeWindow(const char* caption, int width, int height) {
		//Create a window value
		SDL_Window* window = nullptr;

		//Initialize the window with the provided caption and size that is
		// centered on screen
		window = SDL_CreateWindow(
			caption,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width, height,
			SDL_WINDOW_SHOWN
		);

		//If the window failed to initialize, alert the user
		if (!window) {
			cout << "Window could not be created..." << endl;
			cout << SDL_GetError() << endl;
		}

		//Return the newly allocated window
		return window;
	}

	/* Loads an image into an SDL_Texture to be rendered to the screen
	* 
	* Preconditions:
	* - SDL Must be initialized
	* - renderer != nullptr
	* - File must exist at the location specified by filename
	* 
	* Postconditions:
	* - A texture is allocated with the image provided
	* 
	* Params:
	* - filename - the path to the image being loaded onto the texture
	* - renderer - a pointer to the renderer that this texture is a part of
	*/
	inline SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* renderer) {
		//Store the width, height, and bytes per pixel
		int width, height, bytesPerPixel;
		void* data = stbi_load(filename, &width, &height, &bytesPerPixel, 0);

		//Store the image's depth
		int pitch = width * bytesPerPixel;
		pitch = (pitch + 3) & ~3;

		//Store the color channel masks, taking endianness into account
		int32_t RMASK, GMASK, BMASK, AMASK;
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			RMASK = 0x000000FF;
			GMASK = 0x0000FF00;
			BMASK = 0x00FF0000;
			AMASK = (bytesPerPixel == 4) ? 0xFF000000 : 0;
		#else
			RMASK = 0xFF000000;
			GMASK = 0x00FF0000;
			BMASK = 0x0000FF00;
			AMASK = 0x000000FF;
		#endif

		//Load the surface from the data gathered from the stb_image library
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
			data, width, height,
			bytesPerPixel * 8,
			pitch,
			RMASK, GMASK, BMASK, AMASK);

		//If the image was not properly loaded, alert the user and exit
		if (!surface) {
			cout << "LoadImage() failure" << endl;
			return nullptr;
		}

		//Create the texture from the surface, then free it.
		SDL_Texture* texture =
			SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		//Return a pointer to the allocated texture
		return texture;
	}

	/* Loads an image into an SDL_Texture to be rendered to the screen
	*
	* Preconditions:
	* - SDL Must be initialized
	* - renderer != nullptr
	* - File must exist at the location specified by filename
	* - 0 <= alpha <= 255
	*
	* Postconditions:
	* - A texture is allocated with the image provided, with the opacity
	*   adjusted based on the alpha value
	*
	* Params:
	* - filename - the path to the image being loaded onto the texture
	* - renderer - a pointer to the renderer that this texture is a part of
	* - alpha - an integer that represents the opacity of the texture
	*/
	inline SDL_Texture* LoadTexture(
		const char* filename, SDL_Renderer* renderer, int32_t alpha
		) {
		//Store the width, height, and bytes per pixel
		int width, height, bytesPerPixel;
		void* data = stbi_load(filename, &width, &height, &bytesPerPixel, 0);

		//Store the image's depth
		int pitch = width * bytesPerPixel;
		pitch = (pitch + 3) & ~3;

		//Store the color channel masks, taking endianness into account and
		// preserving the opacity value provided
		int32_t RMASK, GMASK, BMASK, AMASK;
		#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			RMASK = 0x000000FF;
			GMASK = 0x0000FF00;
			BMASK = 0x00FF0000;
			AMASK = (bytesPerPixel == 4) ? alpha << 6 : 0;
		#else
			RMASK = 0xFF000000;
			GMASK = 0x00FF0000;
			BMASK = 0x0000FF00;
			AMASK = alpha;
		#endif

		//Load the surface from the data gathered from the stb_image library
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
			data, width, height,
			bytesPerPixel * 8,
			pitch,
			RMASK, GMASK, BMASK, AMASK);

		//If the image was not properly loaded, alert the user and exit
		if (!surface) {
			cout << "LoadImage() failure" << endl;
			return nullptr;
		}

		//Create the texture from the surface, then free it.
		SDL_Texture* texture =
			SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		//Return a pointer to the allocated texture
		return texture;
	}

	inline std::vector<std::string> unpackState(std::string state) {
		std::vector<std::string> textvec;
		
		std::string currstr = "";
		for (char c : state) {
			if (c != ' ')
				currstr += c;
			else {
				textvec.push_back(currstr);
				currstr = "";
			}
		}
		textvec.push_back(currstr);
		
		return textvec;
	}

	template <class T>
	inline bool vecContains(std::vector<T> vec, T element) {
		for (int x = 0; x < vec.size(); x++)
			if (vec[x] == element) return true;
		return false;
	}
}

namespace ctrl {
	class Command {
	protected:
		util::cmd_t type;
	public:
		virtual void execute() = 0;
		virtual void unexecute() = 0;
		util::cmd_t getType() const { return this->type; }
	};
}

#endif