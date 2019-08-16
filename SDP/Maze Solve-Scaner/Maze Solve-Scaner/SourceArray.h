//
//
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
		void setPtColor(const coord_t& x, const coord_t& y, const color_t& color);
		void setPtDistFromStart(const coord_t& x, const coord_t& y, const dist_t& dist);

		Point clonePt(const coord_t& x, const coord_t& y) const;
		Point clonePtSource(const coord_t& x, const coord_t& y) const;
		const color_t& getPtColor(const coord_t& x, const coord_t& y) const;
		dist_t getPtDistFrStart(const coord_t& x, const coord_t& y) const;

		bool inBounds(const Point& pt) const;
		bool inBounds(const coord_t& x, const coord_t& y) const;

		void reset();
		void resetVisistStatus();
		void clear();

		class Iterator;
		class ConstIterator;

		Iterator beg();
		Iterator end();

		ConstIterator cbeg() const;
		ConstIterator cend() const;

	private:
		vector<vector<Point>> arr;
		
	};
}

#include "SourceArrayIterator.h"
#include "SourceArrayConstIterator.h"


#endif

