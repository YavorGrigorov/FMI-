
#ifndef NP_POINT_HEADER_INLCUDED
#define NP_POINT_HEADER_INLCUDED

#include "MazeRules.h"

namespace np {
	typedef int coord_t;
	typedef unsigned color_t;
	struct Point{
		coord_t x, y;
		color_t color;
		
		Point(coord_t x = -1, coord_t y = -1, color_t color = 0xFF000000) 
			: x(x), y(y), color(color) {}
		
		bool operator==(const Point& rhs){
			return 	x == rhs.x 	&&
					y == rhs.y	&&
					color == rhs.color;
		}
		
		bool operator!=(const Point& rhs){
			return !(*this == rhs);
		}
		
		Point& operator+=(const Point& rhs){
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		
		Point operator+(const Point& rhs) const{
			Point temp(*this);
			return temp += rhs;
		}
		
		Point& operator-=(const Point& rhs){
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		
		Point operator-(const Point& rhs) const{
			Point temp(*this);
			return temp -= rhs;
		}
		
		
	};
	
	//
	// Why, oh why are logic operations with higher priority than bitwise...
	static bool sameColor(const Point& lhs, const Point& rhs){
		return (lhs.color & 0x00FFFFFF) == (rhs.color & 0x00FFFFFF);
	}

	static bool cmpColor(color_t rhs, color_t lhs) {
		return (rhs | 0xFF000000) == (lhs | 0xFF000000);
	}
}
#endif

