
#ifndef NP_POINT_HEADER_INLCUDED
#define NP_POINT_HEADER_INLCUDED

#include "MazeRules.h"

namespace np {
	typedef int coord_t;
	typedef unsigned color_t;
	typedef unsigned dist_t;

#define MAX_DIST 0xFFFFFFFF

	struct Point{
		coord_t x, y;
		color_t color;
		dist_t distFromStart;

		Point(coord_t x = -1, coord_t y = -1, color_t color = 0xFF000000, dist_t dist = MAX_DIST);
		
		bool operator==(const Point& rhs);
		
		bool operator!=(const Point& rhs);
		
		Point& operator+=(const Point& rhs);
		
		Point operator+(const Point& rhs) const;
		
		Point& operator-=(const Point& rhs);
		
		Point operator-(const Point& rhs) const;
	};

	bool sameColor(color_t rhs, color_t lhs);
	
	bool sameColor(const Point& lhs, const Point& rhs);

	void visit(Point& pt);

	void unvisit(Point& pt);

	dist_t calcDist(const Point& fr, const Point& to);

}

#endif













