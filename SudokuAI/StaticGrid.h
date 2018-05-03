#pragma once
#include <vector>
template<class T>
class StaticGrid
{
public:
	StaticGrid(unsigned gridWidth, unsigned gridHeight) :
		_gridWidth(gridWidth),
		_gridHeight(gridHeight),
		_rawGrid(_gridWidth * _gridHeight) {

	}
	inline void set(unsigned x, unsigned y, const T& v) {
		_rawGrid[XYToRaw(x, y)] = v;
	}
	inline const T& get(unsigned x, unsigned y) const {
		return _rawGrid[XYToRaw(x, y)];
	}
	inline unsigned getSize() const { return _gridWidth * _gridHeight; }
	inline unsigned getWidth() const { return _gridWidth; }
	inline unsigned getHeight() const { return _gridHeight; }
	~StaticGrid() {};
private:
	unsigned _gridWidth;
	unsigned _gridHeight;
	inline unsigned XYToRaw(unsigned x, unsigned y) const {
		return _gridWidth * y + x;
	}
	std::vector<T> _rawGrid;
};

