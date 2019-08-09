

#include "SourceArrayConstIterator.h"
#include <iostream>
using namespace np;


SourceArray::ConstIterator::ConstIterator(const Point& data, const SourceArray* arr)
	: data(data), arr(arr) {}

SourceArray::ConstIterator& SourceArray::ConstIterator::operator++() {
	if (data.y < arr->getWidth() - 1 || //
		(data.x >= arr->getHeight() - 1 && data.y >= arr->getWidth() - 1))
		++data.y;
	else if (data.y + 1 >= arr->getWidth()) {
		data.y = 0;
		++data.x;
	}
	return *this;
}

SourceArray::ConstIterator SourceArray::ConstIterator::operator++(int) {
	SourceArray::ConstIterator temp(*this);
	++(*this);
	return temp;
}

SourceArray::ConstIterator& SourceArray::ConstIterator::operator--() {
	if (data.y < 1) {
		data.y = arr->getWidth() - 1;
		--data.x;
	}
	else
		--data.y; 
	return *this;
}

SourceArray::ConstIterator SourceArray::ConstIterator::operator--(int) {
	SourceArray::ConstIterator temp(*this);
	--(*this);
	return temp;
}

const Point& SourceArray::ConstIterator::operator*() {
	return getPt(data.x, data.y);
}

const Point* SourceArray::ConstIterator::operator->() {
	return &getPt(data.x, data.y);
}

SourceArray::ConstIterator::operator bool() const {
	return arr->inBounds(data.x, data.y);
}

bool SourceArray::ConstIterator::operator!() const {
	return !(bool)(*this);
}

bool SourceArray::ConstIterator::operator==(const SourceArray::ConstIterator& rhs) const {
	 return arr == rhs.arr && 
			data.x == rhs.data.x && 
			data.y == rhs.data.y;
}

bool SourceArray::ConstIterator::operator!=(const SourceArray::ConstIterator& rhs) const {
	return !(*this == rhs);
}

const Point& SourceArray::ConstIterator::getPt(coord_t x, coord_t y) const {
	return arr->arr[x][y];
}

std::ostream & np::operator<<(std::ostream & out, const SourceArray::ConstIterator & it) {
	out << "(" << it.data.x << " , " << it.data.y << ") color "
		<< std::hex << it.getPt(it.data.x, it.data.y).color << std::dec;
	return out;
}






