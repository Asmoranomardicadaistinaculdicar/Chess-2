#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>

#include "../utils.h"
#include "./pieces.h"

namespace data {
	sq_Piece::sq_Piece(util::piece_t type, util::color_t color, std::string ID) {
		this->ID = ID;
		this->type = type;
		this->color = color;
		this->r = -1;
		this->f = -1;
		this->timesMoved = 0;
	}

	sq_Piece::sq_Piece(util::piece_t type, util::color_t color,
		std::string ID, int r, int f) {
		this->ID = ID;
		this->type = type;
		this->color = color;
		this->r = r;
		this->f = f;
		this->timesMoved = 0;
	}

	std::string sq_Piece::getPos() const {
		char ranks[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		std::string pos = ranks[this->r] + std::to_string(this->f);
		return pos;
	}

	void sq_Piece::moveTo(std::string pos) {
		char ranks[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		int r = 0;
		for (int x = 0; x < 8; x++)
			if (ranks[x] == pos[0]) r = x;
		
		char file = pos[1];
		int f = std::atoi(&file);

		this->r = r;
		this->f = f;
		this->timesMoved++;
	}
}