#ifndef PIECE_H
#define PIECE_H

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "../utils.h"

namespace data {
	class Piece {
	protected:
		util::piece_t type;
		util::color_t color;
		std::string ID;

		int timesMoved;
		bool taken;

	public:
		Piece() {
			this->ID = util::ANONYMOUS;
			this->type = util::PAWN;
			this->color = util::WHITE;
			this->timesMoved = 0;
			this->taken = false;
		}
		Piece(util::piece_t type, util::color_t color, std::string ID) {
			this->ID = ID;
			this->type = type;
			this->color = color;
			this->timesMoved = 0;
			this->taken = false;
		}
		~Piece() {}

		virtual void moveTo(std::string pos) = 0;
		virtual std::string getPos() const = 0;

		std::string getID() const { return this->ID; }
		util::color_t getColor() const { return this->color; }
		util::piece_t getType() const { return this->type; }

		void toggleTake(bool taken) { this->taken = taken; }
		bool isTaken() const { return this->taken; }

		int getMoves() const { return this->timesMoved; }
		void decMoves(int x) { this->timesMoved -= x; }

		virtual void dropPos() = 0;
	};

	class sq_Piece : public Piece {
	private:
		int r, f;

	public:
		sq_Piece(util::piece_t type, util::color_t color, std::string ID);
		sq_Piece(util::piece_t type, util::color_t color, std::string ID, int r, int f);
		~sq_Piece() {}

		int getR() const { return this->r; }
		int getF() const { return this->f; }
		std::string getPos() const;

		void moveTo(std::string pos);
		void dropPos() { this->r = -1; this->f = -1; }
	};
}

namespace ctrl {
	class cmd_MovePiece : public Command {
	private:
		data::Piece* recipient;
		std::string newpos;
		std::string oldpos;

	public:
		cmd_MovePiece(data::Piece* recipient, std::string newpos) {
			this->recipient = recipient;
			this->newpos = newpos;
			this->oldpos = this->recipient->getPos();
			this->type = util::MOVE;
		}

		data::Piece* getRecipient() const { return this->recipient; }
		void execute() {
			this->recipient->moveTo(this->newpos);
		}
		void unexecute() {
			this->recipient->moveTo(this->oldpos);
			this->recipient->decMoves(2);
		}
	};

	class cmd_TakePiece : public Command {
	private:
		data::Piece* recipient;
		std::string pos;

	public:
		cmd_TakePiece(data::Piece* recipient, std::string pos) {
			this->recipient = recipient;
			this->pos = pos;
			this->type = util::TAKE;
		}

		data::Piece* getRecipient() const { return this->recipient; }
		void execute() {
			this->pos = recipient->getPos();
			this->recipient->toggleTake(true);
			this->recipient->dropPos();
			//std::cout << "Piece taken" << std::endl;
		}
		void unexecute() {
			this->recipient->moveTo(this->pos);
			this->recipient->toggleTake(false);
			//std::cout << "Piece untaken" << std::endl;
		}
	};
}

#endif