#include <SDL.h>
#include <iostream>
#include <fstream>

#include "../utils.h"
#include "./Displayable.h"

using std::cout;
using std::endl;

namespace GUI {

	SDL_Rect Displayable::getRect_Static() const {
		//Statically allocate a rect containing raw positional values
		return { this->x, this->y, this->w, this->h };
	}

	SDL_Rect Displayable::getRect_Dynamic() const {
		//Statically allocate a rect and populate it with adjusted values
		return {
			this->x + this->xoffset,
			this->y + this->yoffset,
			this->w + this->woffset,
			this->h + this->hoffset
		};
	}

	bool Displayable::render(SDL_Renderer* renderer) {
		//Ensure the renderer is not null
		if (renderer == nullptr) {
			this->error =
				"Displayable.render(): SDL_Renderer* argument is null";
			return false;
		}
		//Ensure the texture exists
		if (!this->hasTexture()) {
			this->error = "Displayable.render(): No texture found";
			return false;
		}
		//Ensure there is valid sizing information
		if (this->w <= 0 || this->h <= 0) {
			this->error = "Displayable.render(): Invalid width or height";
		}

		//Access the static rect and use it to print the Displayable into
		// the renderer
		SDL_Rect currRect = this->getRect_Static();
		SDL_RenderCopy(renderer, this->img, NULL, &currRect);

		return true;
	}

	bool Displayable::hasTexture() const { return this->img != nullptr; }

	bool Displayable::setTexture(SDL_Renderer* renderer, const char* path) {
		//Dispose of the current texture if one exists
		if (this->hasTexture()) this->dropTexture();
		//Load the new texture and ensure that it's been opened successfully
		this->img = util::LoadTexture(path, renderer);
		if (!this->img) {
			this->error = "Displayable.setTexture(): Texture failed to load";
			return false;
		}
		return true;
	}

	bool Displayable::setTexture(SDL_Texture* img) {
		//Dispose of any existing texture
		if (this->hasTexture()) this->dropTexture();
		//Assign the new texture
		this->img = img;
		return true;
	}

	bool Displayable::dropTexture() {
		if (!this->hasTexture()) {
			this->error = "Displayable.dropTexture(): No texture to destroy";
			return false;
		}
		if (this->destroyOnDrop) SDL_DestroyTexture(this->img);
		this->img = nullptr;
		return true;
	}

	bool Displayable::willDestroyTexture() const {
		return this->destroyOnDrop;
	}

	void Displayable::setDestroy(bool destroy) {
		this->destroyOnDrop = destroy;
	}

	int Displayable::getX() const { return this->x; }
	int Displayable::getY() const { return this->y; }
	int Displayable::getWidth() const { return this->w; }
	int Displayable::getHeight() const { return this->h; }

	void Displayable::setX(int x) { this->x = x; }
	void Displayable::setY(int y) { this->y = y; }
	void Displayable::setWidth(int w) { this->w = w; }
	void Displayable::setHeight(int h) { this->h = h; }

	void Displayable::setRect(SDL_Rect newRect) {
		//Reset offset values
		this->xoffset = 0;
		this->yoffset = 0;
		this->woffset = 0;
		this->yoffset = 0;
		//Store rect information
		this->x = newRect.x;
		this->y = newRect.y;
		this->w = newRect.w;
		this->h = newRect.h;
	}

	void Displayable::setXOffset(int x) { this->xoffset = x; }
	void Displayable::setYOffset(int y) { this->yoffset = y; }
	void Displayable::setWOffset(int w) { this->woffset = w; }
	void Displayable::setHOffset(int h) { this->hoffset = h; }

	std::string Displayable::getError() const { return this->error; }
}