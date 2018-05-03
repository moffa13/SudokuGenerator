#include "SudokuPlate.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

SudokuPlate::SudokuPlate(uint8_t gridSize) : 
	_gridSize(gridSize),
	_grid(gridSize * gridSize, gridSize * gridSize)
{
	fill();
}

SudokuPlate::~SudokuPlate(){}

void SudokuPlate::bench() {

	unsigned took = 0;
	unsigned lowest = std::numeric_limits<unsigned>::max();
	unsigned highest = 0;

	for (unsigned i = 0; i < 5000; ++i) {
		auto now = std::chrono::high_resolution_clock::now();
		fill();
		auto now2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now2 - now);
		auto tookMS = duration.count();
		took += tookMS;
		if (tookMS < lowest) lowest = tookMS;
		if (tookMS > highest) highest = tookMS;
		reset();
	}
	std::cout << "Lowest : " << lowest << "ms" << std::endl;
	std::cout << "Highest : " << highest << "ms" << std::endl;
	std::cout << "Average : " << took / 10000 << "ms" << std::endl;
}

void SudokuPlate::fill() {

	for (unsigned u = 0; u < _gridSize; ++u) {
		for (unsigned v = 0; v < _gridSize; ++v) {

			stuck_info stuck;
			stuck.stuck = false;

			do {
				if (stuck.stuck) {
					coordinate c = getSquareFromCoordinates(stuck.c);
					stuck = fillSquare(c.x, c.y);
				}
				if(!stuck.stuck)
					stuck = fillSquare(v, u);
			} while (stuck.stuck);
			
		}
	}
	
}

stuck_info SudokuPlate::fillSquare(unsigned u, unsigned v) {

	removeSquare(u, v);

	std::unordered_set<unsigned> alreadyInUse;
	
	stuck_info stuck;
	stuck.stuck = false;


	// Each line in the square
	for (unsigned i = 0; i < _gridSize; ++i) {

		// Each column in the square
		for (unsigned j = 0; j < _gridSize; ++j) {
			
			std::unordered_set<unsigned> triedForThisCase;

			unsigned x = _gridSize * u + j;
			unsigned y = _gridSize * v + i;

			unsigned number;
			bool numberCanFit = false;

			do {

				if ((_gridSize * _gridSize) - alreadyInUse.size() == triedForThisCase.size()) {
					stuck.stuck = true;
					// Find what line/column in conflict with the number
					if (_lineUsed[y].find(number) != _lineUsed[y].end()) {
						// Got to find the concerned column
						for (unsigned i = 0; i < _gridSize * _gridSize; ++i) {
							if (_grid.get(i, y) == number) {
								stuck.c.x = i;
								stuck.c.y = y;
							}
						}
					}else if (_columnUsed[x].find(number) != _columnUsed[x].end()) {
						// Got to find the concerned line
						for (unsigned i = 0; i < _gridSize * _gridSize; ++i) {
							if (_grid.get(x, i) == number) {
								stuck.c.x = x;
								stuck.c.y = i;
							}
						}
					}
					return stuck;
				}

				number = randomNumber(1, _gridSize * _gridSize, alreadyInUse);
				numberCanFit = _lineUsed[y].find(number) == _lineUsed[y].end() && _columnUsed[x].find(number) == _columnUsed[x].end();
				if (!numberCanFit)
					triedForThisCase.insert(number);
			} while (!numberCanFit);

			if (numberCanFit) {
				alreadyInUse.insert(number);
				_lineUsed[y].insert(number);
				_columnUsed[x].insert(number);
				_grid.set(x, y, number);
			}
			
		}
	}

	return stuck;

}

coordinate SudokuPlate::getSquareFromCoordinates(coordinate const& c) const {
	return coordinate{
		c.x / _gridSize,
		c.y / _gridSize
	};
}

void SudokuPlate::removeSquare(unsigned u, unsigned v) {
	// Each line in the square
	for (unsigned i = 0; i < _gridSize; ++i) {

		// Each column in the square
		for (unsigned j = 0; j < _gridSize; ++j) {
			std::unordered_set<unsigned> alreadyTried;

			unsigned x = _gridSize * u + j;
			unsigned y = _gridSize * v + i;

			unsigned num = _grid.get(x, y);

			_lineUsed[y].erase(num);
			_columnUsed[x].erase(num);

		}
	}
}


unsigned SudokuPlate::randomNumber(unsigned min, unsigned max, std::unordered_set<unsigned> const& no)
{
	std::random_device random;
	std::mt19937 g(random());
	std::uniform_int_distribution<unsigned> uid(min, max);
	unsigned rand;
	do {
		rand = uid(g);
	} while (no.find(rand) != no.end());
	return rand;
}

void SudokuPlate::print() const {

	unsigned const gridSize = _grid.getWidth();

	for (unsigned i = 0; i < gridSize; ++i) {
		for (unsigned j = 0; j < gridSize; ++j) {
			std::cout << static_cast<int>(_grid.get(j, i));
			if (j != gridSize - 1) {
				std::cout << " | ";
			}
		}
		std::cout << std::endl << "----" << std::endl;
	}
}

void SudokuPlate::reset() {
	for (unsigned u = 0; u < _gridSize; ++u) {
		for (unsigned v = 0; v < _gridSize; ++v) {
			removeSquare(v, u);
		}
	}
}