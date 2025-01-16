#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>

#include "../GUI/Displayable.h"

//Define the Control namespace
namespace ctrl {

	/* This class holds the Displayables in the level and ensures that they
	*   are rendered in the proper order. Each Displayable is associated
	*   with a layer. Each layer is referenced by an integer and are
	*   rendered in ascending order (lower numbered indexes are underneath
	*   higher numbered ones)
	* 
	*  The Displayables in each layer are given a key to be found through.
	*   These keys must be unique among all Displayables in the structure.
	*/
	class PegBar {
	private:
		//This is a vector containing string-Displayable pairs that makes
		// the main data structure of the PegBar class
		std::vector<std::map<std::string, GUI::Displayable*>> layers;
		//This is a vector containing the keys of each layer
		std::vector<std::vector<std::string>> layerKeys;
		//This stores the last error flagged in the PegBar
		std::string error;

		/*Checks whether a layer already exists in the structure
		* 
		* Params:
		* - i - the index of the layer
		* 
		* Returns true IFF layers[i] exists, false OW
		*/
		bool layerExists(uint16_t i) const;

		/*Finds the number of layers present in the structure
		*
		* Returns an unsigned int containing the number of layers that exist
		*/
		uint16_t countLayers() const;

	public:
		/* Default constructor for the PegBar class. Initializes the
		*   layer container with a Layer #0, and empties the error
		*/
		PegBar() {
			this->makeLayer(0);
			this->error = "";
		};
		/* Destroys all Displayables within the structure */
		~PegBar() {
			for (int x = 0; x < this->layers.size(); x++)
				for (int y = 0; y < this->layerKeys[x].size(); y++)
					delete this->layers[x][this->layerKeys[x][y]];
		};

		/* Creates a new layer at the provided index. If the layer does not exist,
		*   then a new entry is created. If the index is more than one greater than
		*   the largest layer value, the new layer's index is created at the top. If
		*   the layer value already exists, the layer at that position (and all layers
		*   on top of it) are shifted up and the new layer is created at the provided
		*   index.
		* 
		* Postconditions:
		* - this->layers.size() = #this->layers.size() + 1
		* - this->layerKeys.size() = #this->layerKeys.size() + 1
		* - All layers indexed > i are shifted up 1 IFF i <= #this->layers.size()
		* 
		* Param:
		* - i - the position of the layer being added
		* 
		* Returns the ID of the newly created layer. Generally, this will be the same
		*  as i, but if i >= this->layers.size() then it will be equal to
		*  this->layers.size()
		*/
		uint16_t makeLayer(uint16_t i);

		/*Destroys a layer at the provided index. If the layer does not exist,
		*  then the function exits. If the layer does exist, all layers above are
		*  shifted downward. If specified, all assets within the layer are destroyed.
		* 
		* Preconditions:
		* - this->layers.size() > 0
		* 
		* Postconditions:
		* - this->layers.size() = #this->layers.size() - 1
		* - this->layerKeys.size() = #this->layerKeys.size() - 1
		* 
		* Param:
		* - i - the index of the layer that is being destroyed
		* - wipeAssets - determines whether the Displayables on the layer will be
		*   destroyed along with the layer
		* 
		* Returns true IFF the layer existed and was successfully removed, false OW
		*/
		bool dropLayer(uint16_t i, bool wipeAssets);
		
		/*Inserts a Displayable into the specified layer, associating it with the
		*  provided Key. 
		* 
		* Preconditions:
		* - key cannot be already associated with a Displayable on any layer
		* - disp cannot point to a Displayable already present in the structure
		* - disp != nullptr
		* - i must index a layer already existant in the structure
		* 
		* Postcondition:
		* - Fill this in later
		* 
		* Params:
		* - key - the string value used to identify the Displayable in the structure
		* - disp - A pointer to a Displayable type object to be displayed
		* - i - the index of the layer the Displayable is being placed onto
		* 
		* Returns true IFF the Displayable was successfully inserted into the
		*  structure, false OW
		*/
		bool insertIntoLayer(std::string key, GUI::Displayable* disp, uint16_t i);

		/*Removes a Displayable that is associated from a provided key from
		*  the structure.
		* 
		* Precondition:
		* - key must be associated with a Displayable in the structure
		* 
		* Postcondition:
		* - the Displayable associated with 'key' is removed from the structure
		* - The Displayable is globally destroyed IFF wipeAsset == true
		* 
		* Params:
		* - key - the string value used to identify the Displayable being searched for
		* - wipeAsset - a value that determines whether the Displayable is globally
		*   destroyed
		* 
		* Returns true IFF the asset existed and was removed successfully, false OW
		*/
		bool removeAsset(std::string key, bool wipeAsset);

		/*Renders all the displayables in the structure onto the provided renderer
		*  so they can be printed to the game surface. Lower layer numbers are added
		*  first, placing them lower on the game screen than higher numbered layers.
		*  If the rendering fails for one of the Displayables in the structure, all
		*  successful ones will still go through. This class will then inherit the
		*  error message of the most recently failed Displayable.
		* 
		* Precondition:
		* - renderer != nullptr
		* 
		* Postconditions:
		* - renderer will now render all Displayables form the strucrure on calls
		*   to SDL_RenderPresent() and similar functions
		* 
		* Params:
		* - renderer - the SDL_Renderer that gets printed onto the game's window
		* 
		* Returns true IFF the render was successful for all Displayables, false OW.
		*/
		bool render(SDL_Renderer* renderer);

		/*Retrieves a Displayable by its associated key.
		* 
		* Params:
		* - key - the string key associated with the Displayable being retrieved
		* 
		* Returns a pointer to the Displayable associated with the key IFF it exists
		*  in the structure, nullptr OW
		*/
		GUI::Displayable* getAssetByKey(std::string key);

		/*Retrieves the layer that the Displayable associated with key is found on.
		* 
		* Params:
		* - key - the string key associated with the Displayable being retrieved
		* 
		* Returns the index of the layer that the Displayable is found on IFF the key
		*  exists in the structure, -1 OW
		*/
		int32_t getLayerByKey(std::string key) const;

		//Accesses the most recent error that has occurred in this Displayable
		std::string getError() const;
	};

	class Level {
	protected:
		SDL_Renderer* renderer;
		//PegBar sprites;

	public:
		virtual void handleClick() = 0;
		virtual void update(std::string& result) = 0;
	};
}

#endif