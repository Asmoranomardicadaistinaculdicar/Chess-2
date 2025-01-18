#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "../utils.h"

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;

//Define the GUI namespace
namespace GUI {

	enum DisplayableType {
		DISP_BASIC,
		DISP_BUTTON
	};

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

		string error; //Stores any error present with the current displayable
		DisplayableType type;

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

			this->type = DISP_BASIC;
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

			this->type = DISP_BASIC;
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

			this->type = DISP_BASIC;
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

			this->type = DISP_BASIC;
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

			this->type = DISP_BASIC;
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

			this->type = DISP_BASIC;
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
		string getError() const;
	};

	/* The class for a clickable rectangular button. It contains all the data
	*   from a basic Displayable, as well as a basic collision definition for when
	*   the mouse is above it. This button has a programmable flag that acts as an ID
	*/
	class Button : public Displayable {
	private:
		string flag;
		map<string, SDL_Texture*> textures;
		string currPose;

	public:
		Button() : Displayable() {
			this->type = DISP_BUTTON;
			this->flag = util::ANONYMOUS;

			this->currPose = util::ANONYMOUS;
		}

		Button(string flag) : Displayable() {
			this->type = DISP_BUTTON;
			this->flag = flag;

			this->currPose = util::ANONYMOUS;
		}

		Button(SDL_Renderer* renderer, map<string, const char*> paths) {
			this->x = 0;
			this->y = 0;
			this->w = -1;
			this->h = -1;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			for (auto i = paths.begin(); i != paths.end(); ++i) {
				this->textures[i->first] = util::LoadTexture(i->second, renderer);
				if (!this->textures[i->first])
					this->error = "Button(): Texture failed to load";
			}
			if (this->textures.empty())
				this->error = "Button(): Textures failed to load";

			this->destroyOnDrop = true;
			this->error = "";

			this->type = DISP_BUTTON;
			this->flag = util::ANONYMOUS;

			map<string, SDL_Texture*>::iterator i;
			this->currPose = i->first;
			this->img = i->second;
		}

		Button(SDL_Renderer* renderer, map<string, const char*> paths, string flag) {
			this->x = 0;
			this->y = 0;
			this->w = -1;
			this->h = -1;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			for (auto i = paths.begin(); i != paths.end(); ++i) {
				this->textures[i->first] = util::LoadTexture(i->second, renderer);
				if (!this->textures[i->first])
					this->error = "Button(): Texture failed to load";
			}
			if (this->textures.empty())
				this->error = "Button(): Textures failed to load";

			this->destroyOnDrop = true;
			this->error = "";

			this->type = DISP_BUTTON;
			this->flag = flag;

			map<string, SDL_Texture*>::iterator i;
			this->currPose = i->first;
			this->img = i->second;
		}

		Button(SDL_Rect rect) :
			Displayable(rect) {
			this->type = DISP_BUTTON;
			this->flag = util::ANONYMOUS;

			this->currPose = util::ANONYMOUS;
		}

		Button(SDL_Rect rect, string flag) :
			Displayable(rect) {
			this->type = DISP_BUTTON;
			this->flag = flag;

			this->currPose = util::ANONYMOUS;
		}

		Button(int w, int h) :
			Displayable(w, h) {
			this->type = DISP_BUTTON;
			this->flag = util::ANONYMOUS;

			this->currPose = util::ANONYMOUS;
		}

		Button(int w, int h, string flag) :
			Displayable(w, h) {
			this->type = DISP_BUTTON;
			this->flag = flag;

			this->currPose = util::ANONYMOUS;
		}

		Button(SDL_Renderer* renderer, map<string, const char*> paths, int w, int h) {
			this->x = 0;
			this->y = 0;
			this->w = w;
			this->h = h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			for (auto i = paths.begin(); i != paths.end(); ++i) {
				this->textures[i->first] = util::LoadTexture(i->second, renderer);
				if (!this->textures[i->first])
					this->error = "Button(): Texture failed to load";
			}
			if (this->textures.empty())
				this->error = "Button(): Textures failed to load";

			this->destroyOnDrop = true;
			this->error = "";

			this->type = DISP_BUTTON;
			this->flag = util::ANONYMOUS;

			map<string, SDL_Texture*>::iterator i;
			this->currPose = i->first;
			this->img = i->second;
		}

		Button(SDL_Renderer* renderer, map<string,
			const char*> paths, int w, int h, string flag) {
			this->x = 0;
			this->y = 0;
			this->w = w;
			this->h = h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			for (auto i = paths.begin(); i != paths.end(); ++i) {
				this->textures[i->first] = util::LoadTexture(i->second, renderer);
				if (!this->textures[i->first])
					this->error = "Button(): Texture failed to load";
			}
			if (this->textures.empty())
				this->error = "Button(): Texture failed to load";

			this->destroyOnDrop = true;
			this->error = "";

			this->type = DISP_BUTTON;
			this->flag = flag;

			map<string, SDL_Texture*>::iterator i;
			this->currPose = i->first;
			this->img = i->second;
		}

		Button(SDL_Renderer* renderer, map<string, const char*> paths, SDL_Rect pos) {
			this->x = pos.x;
			this->y = pos.y;
			this->w = pos.w;
			this->h = pos.h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			for (auto i = paths.begin(); i != paths.end(); ++i) {
				this->textures[i->first] = util::LoadTexture(i->second, renderer);
				if (!this->textures[i->first])
					this->error = "Button(): Texture failed to load";
			}
			if (this->textures.empty())
				this->error = "Button(): Textures failed to load";

			this->destroyOnDrop = true;
			this->error = "";

			this->type = DISP_BUTTON;
			this->flag = util::ANONYMOUS;

			map<string, SDL_Texture*>::iterator i;
			this->currPose = i->first;
			this->img = i->second;
		}

		Button(SDL_Renderer* renderer, map<string, const char*> paths,
			SDL_Rect pos, string flag) {
			this->x = pos.x;
			this->y = pos.y;
			this->w = pos.w;
			this->h = pos.h;

			this->xoffset = 0;
			this->yoffset = 0;
			this->woffset = 0;
			this->hoffset = 0;

			for (auto i = paths.begin(); i != paths.end(); ++i) {
				this->textures[i->first] = util::LoadTexture(i->second, renderer);
				if (!this->textures[i->first])
					this->error = "Button(): Texture failed to load";
			}
			if (this->textures.empty())
				this->error = "Button(): Textures failed to load";

			this->destroyOnDrop = true;
			this->error = "";

			this->type = DISP_BUTTON;
			this->flag = flag;

			map<string, SDL_Texture*>::iterator i;
			this->currPose = i->first;
			this->img = i->second;
		}

		~Button() {
			for (auto i = this->textures.begin(); i != this->textures.end(); ++i) {
				if (i->second != nullptr && this->destroyOnDrop)
					SDL_DestroyTexture(i->second);
			}
		}

		
		/*Detects whether a point falls within the bounds of the Displayable's Rect
		* 
		* Preconditions:
		* - this->w > 0
		* - this->h > 0
		*
		* Params:
		* - x - the x coordinate of the point
		* - y - the y coordinate of the point
		* 
		* Returns true IFF the point noted by (x, y) is in the rect
		*/
		bool collidepoint(int x, int y) const;

		/*Changes the Button's displayed texture so that the proper image is
		*  rendered to the screen
		* 
		* Preconditions:
		* - 'name' must be associated with a SDL_Texture loaded in this->textures
		* 
		* Params:
		* - name - the string-name associated with the SDL_Texture in this->textures
		* 
		* Returns true IFF the texture exists and was successfully assigned, false OW
		*/
		bool setPose(string name);

		/*Provides the string-name associated with the texture currently displayed
		* 
		* Returns the string-name associated with the texture currently displayed
		*/
		string getPose() const;

		/*Inserts an SDL_Texture into the textures list internal to the button
		* 
		* Preconditions:
		* - 'name' cannot already be associated with an SDL_Texture in the button
		* - 'path' must direct to an existant file
		* 
		* Params:
		* - name - the name that will be used to access the provided texture later
		* - path - the location of the image being loaded as a texture
		* 
		* Returns true IFF 'name' does not already reference an existing texture AND 
		*  the image at 'path' was successfully opened and stored
		*/
		bool givePose(string name, const char* path, SDL_Renderer* renderer);

		/*Provides the flag of the button for when it is pressed
		* 
		* Returns the button's flag/identifier
		*/
		string getFlag() const;

		/*Assigns the button's flag
		* 
		* Precondition:
		* this->flag == util::ANONYMOUS
		* 
		* Params:
		* - flag - the flag/identifier being provided to the button
		* 
		* Returns true IFF the flag was set, false OW
		*/
		bool setFlag(string flag);
	};
}

#endif