
#ifndef SOURCE_ARRAY_CONST_ITERATOR_HEADER_INCLUDED
#define SOURCE_ARRAY_CONST_ITERATOR_HEADER_INCLUDED

#include "SourceArray.h"

namespace np {
	class SourceArray::ConstIterator {
	public:
		ConstIterator& operator++();
		ConstIterator operator++(int);

		ConstIterator& operator--();
		ConstIterator operator--(int);

		const Point& operator*();
		const Point* operator->();

		operator bool() const;
		bool operator!() const;

		bool operator==(const ConstIterator& rhs) const;
		bool operator!=(const ConstIterator& rhs) const;

		friend std::ostream& operator<<(std::ostream& out, const ConstIterator& it);

	private:
		ConstIterator(const Point& data, const SourceArray* arr);

		friend class SourceArray;
		const Point& ConstIterator::getPt(coord_t x, coord_t y) const;

		Point data;
		const SourceArray* arr;
	};
}


#endif





