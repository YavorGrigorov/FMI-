

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
		if (arr[x][y].distFromStart > getPtDistFrStart(source.x, source.y) + 1) {
			
			clog << " from (" << arr[x][y].x << " , " << arr[x][y].y << ") to ("
				<< source.x << " , " << source.y << ") dist from start = ";

			arr[x][y].x = source.x;
			arr[x][y].y = source.y;
			arr[x][y].distFromStart = getPtDistFrStart(source.x, source.y) + 1;

			clog << arr[x][y].distFromStart << std::endl;
		}
		else clog << " Failed :: Point has a better source  ( " 
			<< arr[x][y].distFromStart << " < " << getPtDistFrStart(source.x, source.y) + 1 << ")\n";
	}
	else clog << " Failed :: Point out of bounds ( " << source.x << " , " << source.y << " )\n";
}

void np::SourceArray::setPtDistFromStart(const coord_t & x, const coord_t & y, const dist_t & dist) {
	if (inBounds(x, y)) {
		arr[x][y].distFromStart = dist;
	}
}

void SourceArray::setPtColor(const coord_t& x, const coord_t& y, const color_t& color) {
	clog << " Setting point (" << x << " , " << y << ") color";

	if (inBounds(x, y)) {
		clog << " from " << std::hex << arr[x][y].color 
			<< " to " << color << std::dec << std::endl;
		arr[x][y].color = color;
	}
	else clog << " Failed :: Point out of bounds\n";

}

const color_t& SourceArray::getPtColor(const coord_t& x, const coord_t& y) const {
	if (inBounds(x, y))
		return arr[x][y].color;
}

dist_t np::SourceArray::getPtDistFrStart(const coord_t & x, const coord_t & y) const {
	if (!inBounds(x, y)) return MAX_DIST;
	return arr[x][y].distFromStart;
}

Point np::SourceArray::clonePt(const coord_t & x, const coord_t & y) const {
	Point p = { x, y, arr[x][y].color };
	p.distFromStart = arr[x][y].distFromStart;
	return p;
};


Point SourceArray::clonePtSource(const coord_t& x, const coord_t& y) const {
	return clonePt(arr[x][y].x, arr[x][y].y);
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
			arr[i][j].distFromStart = MAX_DIST;
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


