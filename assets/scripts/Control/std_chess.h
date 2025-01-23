#ifndef STD_CHESS_H
#define STD_CHESS_H

#include <SDL.h>
#include <iostream>
#include <vector>
#include <stack>

#include "./Level.h"
#include "../GUI/Displayable.h"
#include "../Data/pieces.h"

namespace ctrl {
	class std_Chess : public Level {
	private:
		std::map<std::string, data::sq_Piece*> pieceList;
		std::string clickedTile;

		std::stack<ctrl::Command*> cmdList;

		data::sq_Piece* getPieceByPos(std::string pos) const;
		std::string getHoveredTile() const;
		bool instantiatePiece(data::sq_Piece* piece);

		std::string getPosByCoords(int r, int f) const;
		std::pair<int, int> getCoordsByPos(std::string pos) const;

		std::vector<std::string> getLegalMoves(data::sq_Piece* piece) const;

		bool inCheck(util::color_t color) const;
		bool inCheckmate(util::color_t color) const;
		bool moveEndangers(data::sq_Piece* piece, std::string dest) const;
		bool moveEscapes(data::sq_Piece* piece, std::string dest) const;

	public:
		std_Chess(SDL_Renderer* renderer);
		~std_Chess() {
			for (auto i = this->pieceList.begin(); i != this->pieceList.end(); i++)
				delete i->second;
			while (!this->cmdList.empty()) {
				delete this->cmdList.top();
				this->cmdList.pop();
			}
		};

		void handleClick();
		void update();
		void render() {
			if (!this->assets.render(this->renderer))
				std::cout << this->assets.getError() << std::endl;
			return;
		}

		void undoAction() {
			if (this->cmdList.empty()) return;

			bool chain = true;
			while (chain) {
				this->cmdList.top()->unexecute();
				if (cmdList.top()->getType() == util::MOVE)
					chain = false;
				this->cmdList.pop();
			}
		}
	};
}

#endif