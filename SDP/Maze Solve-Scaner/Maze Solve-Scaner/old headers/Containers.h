

#ifndef CONTAINERS_INCLUDED_
#define CONTAINERS_INCLUDED_

#include <vector>
#include "Objects.h"

//
/// Scrapped idea
//

typedef unsigned Pixel;
typedef obj::Vector/*<pixel>*/ Segment;
typedef std::vector<Segment> Row;

typedef unsigned Pixel;

typedef obj::Point <Pixel> Coord;

struct Key {
	Coord coords;
	Pixel color;

	Coord center() const {
		return Coord(coords.x + 5, coords.y + 5);
	}
};

// It might change to a struct just because of 
//	the abbcence of setters, getters and so on
class Shape {
	//it's for the single shape class
	// yeah it's not good OOP but that's not my aim
protected:

	// This is taking some of the idea for hash map 
	//	each position in the area signifies a row idx
	//	and contains all the wall segments in it
	// The idea is to make identifing a point on the walls
	//		easier and quicker
	std::vector<Row> arr;
	int firstRow; //used for a bit of compression
	
	Pixel color;

	size_t cnt; //if needed

public:

	Shape(int firstRow = -1, Pixel color = 0) : firstRow(firstRow), color(color), cnt(0) {};

	void setColor(Pixel color) {
		this->color = color;
	}

	Pixel getColor() const {
		return color;
	}

	void setFirstRow(size_t firstRow) {
		this->firstRow = firstRow;
	}

	// For now, all vectors are horisontal
	//	Didn't think of a way to deal with wierd shapes
	void addWallPart(const Segment& wallPart) {
		if (firstRow < 0) 
			setFirstRow(wallPart.getFrom().x);
	
		size_t row = wallPart.getFrom().x;

		arr[row].push_back(wallPart);
		++cnt;

	}

	bool contains(const Coord& pt) const {
		size_t row = pt.x;

		//TODO: Figure a way to make this look-up faster, e.g. if posible O(1)
		//	Idea: Do the same mapping as with the rows
		for (size_t i = 0; i < arr[row].size(); ++i)
			if (arr[row][i].contains(pt)) return true;

		return false;
	}

};

//TODO::
class MapContainer;

#endif











