

#include "Objects.h"

/////// Point /////////

template <class T>
Point<T>& Point<T>::operator+=(const Point<T>& rhs) {
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template <class T>
Point<T> Point<T>::operator+(const Point<T>& rhs) {
	Point<T> temp(*this);
	return temp += rhs;
}

template <class T>
Point<T>& Point<T>::operator-=(const Point<T>& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template <class T>
Point<T> Point<T>::operator-(const Point<T>& rhs) {
	Point<T> temp(*this);
	return temp -= rhs;
}

template <class T>
bool Point<T>::operator==(const Point<T>& rhs) {
	return x == rhs.x && y == rhs.y;
}

template <class T>
bool Point<T>::operator!=(const Point<T>& rhs) {
	return !(*this != rhs);
}


/////// Path ///////////

void Path::addPoint(const Point<size_t>& pt) {
	arr.push_back(pt);
}

const std::vector<Point<size_t>>& Path::getPixelArr() const {
	return arr;
}






