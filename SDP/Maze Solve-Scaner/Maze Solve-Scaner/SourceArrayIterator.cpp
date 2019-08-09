

#include "SourceArrayIterator.h"
#include <iostream>
using namespace np;

SourceArray::Iterator::Iterator(const Point& data, SourceArray* arr)
	: data(data), arr(arr){}

SourceArray::Iterator& SourceArray::Iterator::operator++() {
	if (data.y < arr->getWidth() - 1 || //
		(data.x >= arr->getHeight() - 1 && data.y >= arr->getWidth() - 1))
		++data.y;
	else if (data.y + 1 >= arr->getWidth()) {
		data.y = 0;
		++data.x;
	}
	return *this;
}

SourceArray::Iterator SourceArray::Iterator::operator++(int) {
	SourceArray::Iterator temp(*this);
	++(*this);
	return temp;
}

SourceArray::Iterator& SourceArray::Iterator::operator--() {
	//inBounds should take care of this
	if (data.y < 1) {
		data.y = arr->getWidth() - 1;
		--data.x;
	}
	else
		--data.y;
	return *this;
}

SourceArray::Iterator SourceArray::Iterator::operator--(int) {
	SourceArray::Iterator temp(*this);
	--(*this);
	return temp;
}

Point& SourceArray::Iterator::operator*() {
	return getPt(data.x, data.y);
}

Point* SourceArray::Iterator::operator->() {
	return &getPt(data.x, data.y);
}

SourceArray::Iterator::operator bool() const {
	return arr->inBounds(data);
}

bool SourceArray::Iterator::operator!() const {
	return !(bool)(*this);
}

bool SourceArray::Iterator::operator==(const SourceArray::Iterator& rhs) const {
	return	arr == rhs.arr &&
			data.x == rhs.data.x &&
			data.y == rhs.data.y;
}

bool SourceArray::Iterator::operator!=(const SourceArray::Iterator& rhs) const {
	return !(*this == rhs);
}

Point& SourceArray::Iterator::getPt(coord_t x, coord_t y) const {
	if (!arr->inBounds(x, y)) throw std::exception("index out of bounds SourceArray::Iterator::getPt");
	return arr->arr[data.x][data.y];
}

std::ostream& np::operator<<(std::ostream& out, const SourceArray::Iterator& it) {
	out << "(" << it.data.x << " , " << it.data.y << ") color "
		<< std::hex << it.getPt(it.data.x, it.data.y).color << std::dec;
	return out;
}

