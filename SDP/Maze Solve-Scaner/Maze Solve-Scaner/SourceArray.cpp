

#include "SourceArray.h"
#include <iostream>
using namespace np;
using std::clog;

SourceArray::SourceArray(size_t width, size_t height) {
	clog << " Intializing SArr : Width " << width 
				<< " Height " <<  height << std::endl;
	arr.resize(height);
	for (size_t i = 0; i < height; ++i)
		arr[i].resize(width);
}

void SourceArray::setHeight(size_t height) {
	clog << " Resizing Height from " << this->getHeight()
		<< " to " << height << std::endl;
	arr.resize(height);
}

size_t SourceArray::getHeight() const { 
	return arr.size(); 
}

void SourceArray::setWidth(size_t width) {
	clog << " Resizing Width from " << this->getWidth()
		<< " to " << width << std::endl;
	for (size_t i = 0; i < arr.size(); ++i)
		arr[i].resize(width);
}

size_t SourceArray::getWidth() const { 
	if (arr.empty()) return 0;
	return arr[0].size(); 
}

void SourceArray::setPtSource(const coord_t& x, const coord_t& y, const Point& source) {
	clog << " Setting point (" << x << " , " << y << ") source";
	if (inBounds(x, y) && inBounds(source)) {
		clog << " from (" << arr[x][y].first.x << " , " << arr[x][y].first.y << ") to ("
			<< x << " , " << y << ") \n";
		arr[x][y].first.x = source.x;
		arr[x][y].first.y = source.y;
	}
	else clog << " Failed :: Point out of bounds\n";
}

void SourceArray::setPtColor(const coord_t& x, const coord_t& y, unsigned color) {
	clog << " Setting point (" << x << " , " << y << ") color";

	if (inBounds(x, y)) {
		clog << " from " << std::hex << arr[x][y].first.color 
			<< " to " << color << std::dec << std::endl;
		arr[x][y].color = color;
	}
	else clog << " Failed :: Point out of bounds\n";

}

const color_t& SourceArray::getPtColor(const coord_t& x, const coord_t& y) const {
	if (inBounds(x, y))
		return arr[x][y].first.color;
}

const Point& SourceArray::getPtSource(const coord_t& x, const coord_t& y) const {
	if (inBounds(x, y))
		return arr[x][y].first;
}

bool SourceArray::inBounds(const coord_t& x, const coord_t& y) const {
	return 	!arr.empty()  &&
		x >= 0			  &&
		x < getHeight()	  &&
		y >= 0			  &&
		y < getWidth();
}

void SourceArray::reset() {
	clog << " Reseting Source array (e.g. making all pt's unvisited and with invalid sources)\n";
	for (size_t i = 0; i < arr.size(); ++i)
		for (size_t j = 0; j < arr[i].size(); ++j) {
			arr[i][j].x = -1;
			arr[i][j].y = -1;
			arr[i][j].color = (arr[i][j].color | 0xFF000000);
		}
}

void SourceArray::clear() {
	clog << " Clearing Source array\n";
	arr.clear();
	arr.shrink_to_fit();// see if there is another way to kill the used memory
}

bool SourceArray::inBounds(const Point& pt) const {
	return inBounds(pt.x, pt.y);
}


SourceArray::Iterator SourceArray::beg() {
	return SourceArray::Iterator(Point(0,0), this);
}

SourceArray::Iterator SourceArray::end() {
	Point pt;
	pt.x = arr.size() - 1;
	pt.y = arr[arr.size() - 1].size();
	return SourceArray::Iterator(pt, this);
}

SourceArray::ConstIterator SourceArray::cbeg() const {
	return SourceArray::ConstIterator(Point(0,0), this);
}

SourceArray::ConstIterator SourceArray::cend() const {
	Point pt;
	pt.x = arr.size() - 1;
	pt.y = arr[arr.size() - 1].size();
	return SourceArray::ConstIterator(pt, this);
}


