#include <SDL.h>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <string>

#include "./Level.h"
#include "./std_chess.h"
#include "../GUI/Displayable.h"
#include "../Data/boards.h"
#include "../Data/pieces.h"

namespace ctrl {
	std_Chess::std_Chess(SDL_Renderer* renderer) {
		this->renderer = renderer;
		SDL_GetMouseState(&this->mx, &this->my);

		this->clickedTile = util::ANONYMOUS;

		std::map<std::string, const char*> dark_paths;
		dark_paths["neutral"] = "./assets/texture/tiles/square/dark_neutral.png";
		dark_paths["hover"] = "./assets/texture/tiles/square/dark_hover.png";
		dark_paths["clicked"] = "./assets/texture/tiles/square/dark_clicked.png";

		std::map<std::string, const char*> light_paths;
		light_paths["neutral"] = "./assets/texture/tiles/square/light_neutral.png";
		light_paths["hover"] = "./assets/texture/tiles/square/light_hover.png";
		light_paths["clicked"] = "./assets/texture/tiles/square/light_clicked.png";

		char ranks[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

		this->assets.insertIntoLayer(
			"bkgd",
			new GUI::Displayable(
				this->renderer,
				dark_paths["clicked"],
				{ 40, 50, 620, 620 }
			),
			0
		);

		this->assets.makeLayer(1);
		bool dark = 0;
		for (int x = 0; x < 64; x++) {
			int r = x % 8;
			int f = x / 8;
			std::string id = this->getPosByCoords(r, f);

			GUI::Button* tile =
				new GUI::Button({ 50 + r * 75, 60 + f * 75, 75, 75 }, id);

			if (dark) {
				if (r != 7) dark = 0;
				tile->givePose("neutral", light_paths["neutral"], this->renderer);
				tile->givePose("hover", light_paths["hover"], this->renderer);
				tile->givePose("clicked", light_paths["clicked"], this->renderer);
			}
			else {
				if (r != 7) dark = 1;
				tile->givePose("neutral", dark_paths["neutral"], this->renderer);
				tile->givePose("hover", dark_paths["hover"], this->renderer);
				tile->givePose("clicked", dark_paths["clicked"], this->renderer);
			}

			tile->setPose("neutral");
			this->assets.insertIntoLayer(id, tile, 1);
		}

		this->assets.makeLayer(2);

		data::sq_Piece* piece;
		piece = new data::sq_Piece(util::ROOK, util::WHITE, "WR1", 0, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::ROOK, util::WHITE, "WR2", 7, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::KNIGHT, util::WHITE, "WN1", 1, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::KNIGHT, util::WHITE, "WN2", 6, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::BISHOP, util::WHITE, "WB1", 2, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::BISHOP, util::WHITE, "WB2", 5, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::KING, util::WHITE, "WK1", 3, 7);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::QUEEN, util::WHITE, "WQ1", 4, 7);
		this->instantiatePiece(piece);
		for (int x = 0; x < 8; x++) {
			piece = new data::sq_Piece(
				util::PAWN, util::WHITE, "WP" + std::to_string(x), x, 6
			);
			this->instantiatePiece(piece);
		}

		piece = new data::sq_Piece(util::ROOK, util::BLACK, "BR1", 0, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::ROOK, util::BLACK, "BR2", 7, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::KNIGHT, util::BLACK, "BN1", 1, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::KNIGHT, util::BLACK, "BN2", 6, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::BISHOP, util::BLACK, "BB1", 2, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::BISHOP, util::BLACK, "BB2", 5, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::KING, util::BLACK, "BK1", 3, 0);
		this->instantiatePiece(piece);
		piece = new data::sq_Piece(util::QUEEN, util::BLACK, "BQ1", 4, 0);
		this->instantiatePiece(piece);
		for (int x = 0; x < 8; x++) {
			piece = new data::sq_Piece(
				util::PAWN, util::BLACK, "BP" + std::to_string(x), x, 1
			);
			this->instantiatePiece(piece);
		}
	}

	bool std_Chess::instantiatePiece(data::sq_Piece* piece) {
		if (this->getPieceByPos(piece->getPos())) return false;

		const char* path = "";
		if (piece->getColor() == util::WHITE) {
			path = util::wpaths[piece->getType()];
		}
		else {
			path = util::bpaths[piece->getType()];
		}

		this->pieceList[piece->getID()] = piece;
		this->assets.insertIntoLayer(
			piece->getID(),
			new GUI::Displayable(
				this->renderer, path,
				{ 50 + 75 * piece->getR(), 60 + 75 * piece->getF(), 75, 75 }
			),
			2
		);
		return true;
	}

	void std_Chess::handleClick() {
		SDL_GetMouseState(&this->mx, &this->my);

		for (std::string key : this->assets.getKeys(1)) {
			GUI::Button* tile = (GUI::Button*)this->assets.getAssetByKey(key);

			if (tile->collidepoint(this->mx, this->my)) {
				if (this->getPieceByPos(this->clickedTile) != nullptr) {
					data::sq_Piece* piece = this->getPieceByPos(this->clickedTile);

					std::string newpos = this->getHoveredTile();
					this->cmdList.push(
						new cmd_MovePiece(piece, newpos)
					);
					this->cmdList.top()->execute();

					this->clickedTile = util::ANONYMOUS;
				}
				else {
					if (this->getPieceByPos(this->getHoveredTile()) != nullptr) {
						this->clickedTile = key;
						tile->setPose("clicked");
					}
				}
			}

			else tile->setPose("neutral");
		}

		return;
	}

	void std_Chess::update() {
		SDL_GetMouseState(&this->mx, &this->my);

		std::vector<std::string> legalMoves =
			this->getLegalMoves(this->getPieceByPos(this->clickedTile));

		for (std::string key : this->assets.getKeys(1)) {
			GUI::Button* tile = (GUI::Button*)this->assets.getAssetByKey(key);

			if (key == this->clickedTile) continue;

			if (util::vecContains(legalMoves, key)) {
				if (tile->collidepoint(this->mx, this->my)) tile->setPose("clicked");
				else tile->setPose("hover");
			}
			
			else {
				if (tile->collidepoint(this->mx, this->my)) tile->setPose("hover");
				else tile->setPose("neutral");
			}
		}

		for (std::string key : this->assets.getKeys(2)) {
			GUI::Displayable* pieceDisp = this->assets.getAssetByKey(key);
			data::sq_Piece* pieceRep = this->pieceList.at(key);

			pieceDisp->setX(50 + 75 * pieceRep->getR());
			pieceDisp->setY(60 + 75 * pieceRep->getF());
		}

		return;
	}

	data::sq_Piece* std_Chess::getPieceByPos(std::string pos) const {
		for (auto i = this->pieceList.begin(); i != this->pieceList.end(); i++)
			if (i->second->getPos() == pos) return i->second;
		return nullptr;
	}

	std::string std_Chess::getHoveredTile() const {
		int r = this->mx;
		int f = this->my;

		r -= 50;
		f -= 60;

		r /= 75;
		f /= 75;

		return this->getPosByCoords(r, f);
	}

	std::string std_Chess::getPosByCoords(int r, int f) const {
		char ranks[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		std::string pos = ranks[r] + std::to_string(f);
		return pos;
	}

	std::pair<int, int> std_Chess::getCoordsByPos(std::string pos) const {
		char ranks[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		int r = 0;
		for (int x = 0; x < 8; x++)
			if (ranks[x] == pos[0]) r = x;
		int f = std::atoi((const char*)(pos)[1]);
		return std::pair<int, int>(r, f);
	}

	std::vector<std::string> std_Chess::getLegalMoves(data::sq_Piece* piece) const {
		std::vector<std::string> legalMoves;
		if (piece == nullptr) return legalMoves;

		if (piece->getType() == util::PAWN) {
			;
		}

		else if (piece->getType() == util::ROOK) {
			;
		}

		else if (piece->getType() == util::KNIGHT) {
			;
		}

		else if (piece->getType() == util::BISHOP) {
			;
		}

		else if (piece->getType() == util::QUEEN) {
			;
		}

		else if (piece->getType() == util::KING) {
			;
		}

		return legalMoves;
	}
}