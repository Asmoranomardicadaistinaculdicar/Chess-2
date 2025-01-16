#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdint.h>

#include "../GUI/Displayable.h"
#include "./Level.h"

using GUI::Displayable;
using std::string;

//Define the Control namespace
namespace ctrl {
	/*-------------------------------*/
	/*   Peg Bar Class Definitions   */
	/*-------------------------------*/

	//Private
	bool PegBar::layerExists(uint16_t i) const { return i < this->layers.size(); }
	uint16_t PegBar::countLayers() const { return this->layers.size(); }

	//Public
	uint16_t PegBar::makeLayer(uint16_t i) {
		//Store the index, may be changed later
		uint16_t index = i;
		//Check whether the layer already exists
		bool exists = this->layerExists(i);

		//Create new entries in the layer and layer key structures
		this->layers.push_back( std::map<string, Displayable*>() );
		this->layerKeys.push_back(std::vector<string>());

		//If the layer already exists, shift everything backward one
		if (exists) {
			//Swap the maps at the locations until the new entry reaches
			// the desired spot
			for (int x = this->layers.size() - 1; x > i; x--) {
				std::map<string, Displayable*> tempL = this->layers[x];
				this->layers[x] = this->layers[x - 1];
				this->layers[x - 1] = tempL;

				std::vector<string> tempK = this->layerKeys[x];
				this->layerKeys[x] = this->layerKeys[x - 1];
				this->layerKeys[x - 1] = tempK;
			}
		}

		//If the layer does not exist, return the index of the new cell
		else index = this->layers.size() - 1;

		//Return the true index of the new layer
		return index;
	}
	bool PegBar::dropLayer(uint16_t i, bool wipeAssets) {
		if (!this->layerExists(i)) {
			this->error = "PegBar.dropLayer(): Layer cannot be removed, does not exist";
			return false;
		}

		//Swap the locations forward until the target layer is on the top of
		// the structure
		for (int x = i; x < this->layers.size() - 1; x++) {
			std::map<string, Displayable*> tempL = this->layers[x];
			this->layers[x] = this->layers[x + 1];
			this->layers[x + 1] = tempL;

			std::vector<string> tempK = this->layerKeys[x];
			this->layerKeys[x] = this->layerKeys[x + 1];
			this->layerKeys[x + 1] = tempK;
		}

		//Destroy the contents of the layers box if applicable
		if (wipeAssets) {
			uint16_t t = this->layers.size() - 1;
			for (int x = 0; x < this->layerKeys[t].size(); x++)
				delete this->layers[t][this->layerKeys[t][x]];
		}

		//Pop the final layer off of the structure
		this->layers.pop_back();
		this->layerKeys.pop_back();

		return true;
	}
	bool PegBar::insertIntoLayer(string key, Displayable* disp, uint16_t i) {
		//Ensure that the layer already exists
		if (!layerExists(i)) {
			this->error = "PegBar.insertIntoLayer(): Layer not initialized";
			return false;
		}

		//Ensure that the key is not already in use
		for (int x = 0; x < this->layers.size(); x++)
			if (this->layers[x].count(key)) {
				this->error = "PegBar.insertIntoLayer(): Key already in use";
				return false;
			}

		//Ensure that the displayable is not null
		if (disp == nullptr) {
			this->error = "PegBar.insertIntoLayer(): Cannot insert null Displayable";
			return false;
		}

		//Insert the Displayable pointer into the layers table with its key
		this->layers[i].insert(std::pair<string, Displayable*>(key, disp));
		//Insert the key into the layerKey list
		this->layerKeys[i].push_back(key);

		return true;
	}
	bool PegBar::removeAsset(string key, bool wipeAsset) {
		//Get the layer index of the asset being removed
		int32_t i = this->getLayerByKey(key);
		if (i == -1) {
			this->error = "PegBar.removeAsset(): Asset not found";
			return false;
		}

		//Remove the index from the layer
		if (wipeAsset) delete this->layers[i][key];
		this->layers[i].erase(key);

		//Remove the key from the key list
		int del = -1;
		for (int x = 0; x < this->layerKeys[i].size() - 1; x++)
			if (this->layerKeys[i][x] == key) { del = x; break; }

		//Swap the target Displayable
		this->layerKeys[i][del] = this->layerKeys[i][this->layerKeys[i].size() - 1];
		this->layerKeys.pop_back();

		return true;
	}
	bool PegBar::render(SDL_Renderer* renderer) {
		bool failed = false;

		//Iterate through each layer in the structure
		for (int x = 0; x < this->layers.size(); x++) {
			//For each layer, iterate through the Displayables and render them
			for (int y = 0; y < this->layerKeys[x].size(); y++) {
				string currKey = this->layerKeys[x][y];
				bool success = this->layers[x][currKey]->render(renderer);
				if (!success) {
					this->error = this->layers[x][currKey]->getError();
					bool failed = true;
				}
			}
		}

		//Return the opposite of whether it failed
		return !failed;
	}
	Displayable* PegBar::getAssetByKey(string key) {
		//Access the index of the layer containing the Displayable
		int32_t l = this->getLayerByKey(key);
		//If there was no layer found, return nullptr
		if (l == -1) {
			this->error = "PegBar.getAssetByKey(): Cannot find key";
			return nullptr;
		}

		//Return the pointer associated with the key
		return this->layers[l][key];
	}
	int32_t PegBar::getLayerByKey(string key) const {
		//Initialize the index to a null value
		int32_t index = -1;
		//Iterate through to find the index of the provided key
		for (int x = 0; x < this->layers.size(); x++)
			if (this->layers[x].count(key)) index = x;
		//Return the index. If not found, this value will remain -1
		return index;
	}
	string PegBar::getError() const { return this->error; }
}