//
// Are objects gonna have diff part of them different sources or
// will they have the same source, e.g. if it's a ket (20x20)
// the walls of the key point to the entry pt of the key 
// (the first pt that got to the key)
//
#ifndef NP_SOURCE_ARRAY_HEADER_INCLUDED
#define NP_SOURCE_ARRAY_HEADER_INCLUDED

#include "Point.h"
#include <vector>

using std::vector;

namespace np {
	class SourceArray {
	public:
		SourceArray(size_t width = 0, size_t height = 0);

		void setHeight(size_t height);
		void setWidth(size_t width);
		size_t getHeight() const;
		size_t getWidth() const;

		void setPtSource(const coord_t& x, const coord_t& y, const Point& source);
		void setPtColor(const coord_t& x, const coord_t& y, unsigned color);

		const Point& getPtSource(const coord_t& x, const coord_t& y) const;
		const color_t& getPtColor(const coord_t& x, const coord_t& y) const;

		bool inBounds(const Point& pt) const;
		bool inBounds(const coord_t& x, const coord_t& y) const;

		void reset();
		void clear();

		class Iterator;
		class ConstIterator;

		Iterator beg();
		Iterator end();

		ConstIterator cbeg() const;
		ConstIterator cend() const;

	private:
		vector<vector<Point>> arr;//the void* idea doesn't work with resizing (address)
	};
}

#include "SourceArrayIterator.h"
#include "SourceArrayConstIterator.h"


#endif

