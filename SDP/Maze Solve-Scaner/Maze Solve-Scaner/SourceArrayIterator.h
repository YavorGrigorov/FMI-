
#ifndef SOURCE_ARRAY_ITERATOR_HEADER_INCLUDED
#define SOURCE_ARRAY_ITERATOR_HEADER_INCLUDED

#include "SourceArray.h"

namespace np {
	class SourceArray::Iterator {
	public:
		Iterator& operator++();
		Iterator operator++(int);

		Iterator& operator--();
		Iterator operator--(int);

		Point& operator*();
		Point* operator->();

		operator bool() const;
		bool operator!() const;

		bool operator==(const Iterator& rhs) const;
		bool operator!=(const Iterator& rhs) const;

		friend std::ostream& operator<<(std::ostream& out, const Iterator& it);
	private:
		friend class SourceArray;

		Iterator(const Point& data, SourceArray* arr);
		Point& getPt(coord_t x, coord_t y) const;

		Point data;
		SourceArray* arr;
	};

	
}


#endif


