#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

#include <SDL.h>
#include <iostream>
#include <fstream>

#include "../utils.h"

using std::cout;
using std::endl;

//Define the GUI namespace
namespace GUI {

	/* The base class for an object that can be displayed on screen. Contains
	*   position and size information/offsets, as well as a texture to render.
	*   This should generally not be directly invoked directly but inherited
	*   in more specific Displayable types.
	*/
	class Displayable {
	protected:
		int x, y; //Positional coordinates
		int w, h; //Sizing information

		int xoffset, yoffset; //Positional offsets for the dynamic rect
		int woffset, hoffset; //Sizing information for the dynamic rect

		SDL_Texture* img; //The texture to be rendered

		bool destroyOnDrop; //Whether the texture will be destroyed when
		// dropTexture() is called. True by default

		std::string error; //Stores any error present with the current displayable

	public:
		/*Default constructor, initializes all fields to null values*/
		Displayable() {
			this->x = 0;
			this->y = 0;
			this->w = -1;
			this->h = -1;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			this->img = nullptr;
			this->destroyOnDrop = true;
			this->error = "";
		}

		/*Texture constructor, loads the texture from the path and sets positional
		* fields to null values
		*/
		Displayable(SDL_Renderer* renderer, const char* path) {
			this->x = 0;
			this->y = 0;
			this->w = -1;
			this->h = -1;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			this->img = util::LoadTexture(path, renderer);
			if (this->img == nullptr)
				this->error = "Displayable(): Texture failed to load";
			this->destroyOnDrop = true;
			this->error = "";
		}

		/*Positional constructor, unpacks the provided rect and initializes
		* the texture field to a null value
		*/
		Displayable(SDL_Rect rect) {
			this->x = rect.x;
			this->y = rect.y;
			this->w = rect.w;
			this->h = rect.h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			this->img = nullptr;
			this->destroyOnDrop = true;
			this->error = "";
		}

		/*Sized constructor, stores the width and height, initializes the texture
		* and position fields to null values
		*/
		Displayable(int w, int h) {
			this->x = 0;
			this->y = 0;
			this->w = w;
			this->h = h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			this->img = nullptr;
			this->destroyOnDrop = true;
			this->error = "";
		}

		/*Texture and size constructor, loads the texture and stores sizing
		* data, initializes the position fields to null values
		*/
		Displayable(SDL_Renderer* renderer, const char* path, int w, int h) {
			this->x = 0;
			this->y = 0;
			this->w = w;
			this->h = h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			this->img = util::LoadTexture(path, renderer);
			if (this->img == nullptr)
				this->error = "Displayable(): Texture failed to load";
			this->destroyOnDrop = true;
			this->error = "";
		}

		/*Complete constructor, loads the texture and unpacks the provided rect
		* into positional data
		*/
		Displayable(SDL_Renderer* renderer, const char* path, SDL_Rect pos) {
			this->x = pos.x;
			this->y = pos.y;
			this->w = pos.w;
			this->h = pos.h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			this->img = util::LoadTexture(path, renderer);
			if (this->img == nullptr)
				this->error = "Displayable(): Texture failed to load";
			this->destroyOnDrop = true;
			this->error = "";
		}

		//Destroys the texture if speficied to prevent memory leaks
		~Displayable() {
			if (this->hasTexture() && this->destroyOnDrop)
				SDL_DestroyTexture(this->img);
		}

		/*Accesses the rect without offsets, used primarily for rendering
		*
		* Returns a SDL_Rect containing raw positional/sizing information
		*/
		SDL_Rect getRect_Static() const;

		/*Accesses the rect with offsets, used primarily for collision detection
		*
		* Returns an SDL_Rect containing modified positiona/sizing information
		*/
		SDL_Rect getRect_Dynamic() const;

		/*Renders the Displayable with the proper texture and position/sizing
		*
		* Preconditions:
		* - renderer != nullptr
		* - this->texture != nullptr
		* - this->w > 0
		* - this->h > 0
		*
		* Postconditions:
		* - renderer will now render this Displayable on calls to
		*   SDL_RenderPresent() and similar functions
		*
		* Params:
		* - renderer is the SDL_Renderer that gets printed onto the
		*   game's window
		*
		* Returns true IFF img != nullptr && w > 0 && h > 0, false OW
		*/
		bool render(SDL_Renderer* renderer);

		/*Checks that the Displayable has been given a texture
		*
		* Returns true IFF img != nullptr, false OW
		*/
		bool hasTexture() const;

		/*Assigns a texture to the Displayable.
		* Note: If an image is provided this way and this Displayable is not set
		*  to destroy textures on disposal, then memory leaks may be created.
		*
		* Preconditions:
		* - renderer != null
		* - path != null
		* - An image must be present at the location specified by 'path'
		*
		* Postcondition:
		* - this->img is now populated with a newly allocated SDL_Texture
		*
		* Params:
		* - renderer is the SDL_Renderer that gets printed onto the
		*   game's window
		* - path is the path (relative to the game's root folder) that
		*   the texture resides at.
		*
		* Returns true IFF the texture was successfully loaded, false OW
		*/
		bool setTexture(SDL_Renderer* renderer, const char* path);

		/*Assigns a texture to the Displayable.
		* Note: If an image is provided this way and this Displayable is set to
		*  destroy textures on disposal, all other references to img will fail.
		*
		* Preconditions:
		* - img != null
		*
		* Postcondition:
		* - this->img is now pointing to the provided SDL_Texture
		*
		* Params:
		* - img is the SDL_Texture being provided to the Displayable
		*
		* Returns true always to make compatible with the other setTexture
		*  function
		*/
		bool setTexture(SDL_Texture* img);

		/*Disposes of the texture currently stored in the Displayable. If the
		*  displayable is set to destroy textures on disposal (as is default)
		*  then the texture will be globally destroyed. Otherwise, it simply has
		*  its reference removed.
		*
		* Precondition:
		* - this->img != nullptr
		*
		* Postcondition:
		* - this->img == nullptr
		* - The texture referenced by this->img is globally destroyed IFF
		*  this Displayable is set to destroy textures on disposal.
		*
		* Returns true IFF there was a texture to remove, false OW
		*/
		bool dropTexture();

		/*Checks whether the Displayable is set to destroy textures on disposal
		*
		* Returns true IFF the displayable is set to destroy textures on disposal
		*/
		bool willDestroyTexture() const;

		/*Toggles whether the Displayable destroys textures on disposal
		*
		* Params:
		* - destroy is the true/false value that the Displayable will update
		*   its setting to.
		*/
		void setDestroy(bool destroy);

		/*Accessors for positional and sizing information*/
		int getX() const;
		int getY() const;
		int getWidth() const;
		int getHeight() const;

		/*Mutators for positional and sizing information*/
		void setX(int x);
		void setY(int y);
		void setWidth(int w);
		void setHeight(int h);

		/*Overrides the Displayable's static and dynamic rects
		*
		* Postconditions:
		* - All offset values are now 0;
		* - this->x == newRect.x, this->y == newRect.y
		* - this->w == newRect.w, this->h == newRect.h
		*/
		void setRect(SDL_Rect newRect);

		/*Mutators for offset information*/
		void setXOffset(int x);
		void setYOffset(int y);
		void setWOffset(int w);
		void setHOffset(int h);

		//Accesses the most recent error that has occurred in this Displayable
		std::string getError() const;
	};
}

#endif