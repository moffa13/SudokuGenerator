#pragma once
#include "StaticGrid.h"
#include <list>
#include <unordered_set>
#include <unordered_map>

typedef struct {
	unsigned x, y;
} coordinate;

typedef struct {
	bool stuck;
	coordinate c;
} stuck_info;

class SudokuPlate
{
public:
	SudokuPlate(uint8_t gridSize);
	void bench();
	void print() const;
	~SudokuPlate();
private:
	void fill();
	void reset();
	inline coordinate getSquareFromCoordinates(coordinate const& c) const;
	stuck_info fillSquare(unsigned i, unsigned j);
	void removeSquare(unsigned i, unsigned j);
	static unsigned randomNumber(unsigned min, unsigned max, std::unordered_set<unsigned> const& no);
	std::unordered_map<unsigned, std::unordered_set<unsigned>> _lineUsed;
	std::unordered_map<unsigned, std::unordered_set<unsigned>> _columnUsed;
	unsigned _gridSize;
	StaticGrid<uint8_t> _grid;
};

