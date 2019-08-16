

#include "Logger.h"

#include "SourceArray.h"
#include <iostream>
using namespace np;



SourceArray::SourceArray(size_t width, size_t height) {
	LOG_TEXT(" Intializing SArr : Width ");
	LOG_TEXT(width);
	LOG_TEXT(" Height "); LOG_TEXT(height); LOG_ENDL;
	arr.resize(height);
	for (size_t i = 0; i < height; ++i)
		arr[i].resize(width);
}

void SourceArray::setHeight(size_t height) {
	LOG_TEXT(" Resizing Height from ");
	LOG_TEXT(this->getHeight());
	LOG_TEXT(" to "); LOG_TEXT(height) LOG_ENDL;
	arr.resize(height);
}

size_t SourceArray::getHeight() const { 
	return arr.size(); 
}

void SourceArray::setWidth(size_t width) {
	LOG_TEXT(" Resizing Width from ");
	LOG_TEXT(this->getWidth());
	LOG_TEXT(" to "); LOG_TEXT(width) LOG_ENDL;
	for (size_t i = 0; i < arr.size(); ++i)
		arr[i].resize(width);
}

size_t SourceArray::getWidth() const { 
	if (arr.empty()) return 0;
	return arr[0].size(); 
}

void SourceArray::setPtSource(const coord_t& x, const coord_t& y, const Point& source) {
	LOG_POINT_CHANGE_SOURCE(Point(x, y), arr[x][y], source);

	if (inBounds(x, y) && inBounds(source)) {
		arr[x][y].x = source.x;
		arr[x][y].y = source.y;

		LOG_POINT_DIST_FROM_START(arr[x][y], (*this));
	}
	else LOG_POINT_OUT_OF_BOUNDS(source);
}

void np::SourceArray::setPtDistFromStart(const coord_t & x, const coord_t & y, const dist_t & dist) {
	if (inBounds(x, y)) {
		arr[x][y].distFromStart = dist;
	}
}

void SourceArray::setPtColor(const coord_t& x, const coord_t& y, const color_t& color) {
	LOG_TEXT(" Setting point "); LOG_POINT_COORD(Point(x, y)); LOG_TEXT( " color");

	if (inBounds(x, y)) {
		LOG_POINT_COLOR(arr[x][y], (*this));
		LOG_TEXT(" to "); LOG_COLOR(color);LOG_ENDL;
		arr[x][y].color = color;
	}
	else LOG_POINT_OUT_OF_BOUNDS(Point(x,y));

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
	Point p;
	if (inBounds(x, y)) {
		p = { x, y, arr[x][y].color };
		p.distFromStart = arr[x][y].distFromStart;
	}
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

void SourceArray::resetVisistStatus() {
	LOG_TEXT(" Reseting SourceArray's point's visit status\n");
	for (size_t i = 0; i < arr.size(); ++i)
		for (size_t j = 0; j < arr[i].size(); ++j)
			unvisit(arr[i][j]);
}

void SourceArray::reset() {
	LOG_TEXT(" Reseting Source array (e.g. making all pt's unvisited and with invalid sources)\n");
	for (size_t i = 0; i < arr.size(); ++i)
		for (size_t j = 0; j < arr[i].size(); ++j) {
			arr[i][j].x = -1;
			arr[i][j].y = -1;
			unvisit(arr[i][j]);
			arr[i][j].distFromStart = MAX_DIST;
		}
}

void SourceArray::clear() {
	LOG_TEXT(" Clearing Source array\n");
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


