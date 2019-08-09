
#ifndef OBJECTS_HEADER_INCLUDED_
#define OBJECTS_HEADER_INCLUDED_

#include <vector>
// TODO: Before sending the code, delete all the beginer stuff



//typedef unsigned T;
template <class T>
struct Point {
	T x, y;

	Point() = default;
	Point(const T& x, const T& y) : x(x), y(y) {};

	Point& operator+=(const Point& rhs);

	Point operator+(const Point& rhs);

	Point& operator-=(const Point& rhs);

	Point operator-(const Point& rhs);

	bool operator==(const Point& rhs);

	bool operator!=(const Point& rhs);

	operator std::pair<T, T>() const {
		return std::make_pair(x, y);
	}
};

//Sadness insues

class Path {
	std::vector<Point<size_t>> arr;

public:

	void addPoint(const Point<size_t>& pt);

	const std::vector<Point<size_t>>& getPixelArr() const;
};

union Pixel {
	unsigned c;

	char p[4];

	Pixel() = default;
	template <class T>
	Pixel(const T& i) : c(i) {

	}

	operator unsigned&() { return c; }

	template <class T>
	Pixel operator&(const T& rhs) const {
		return Pixel(c & rhs);
	}

	template <class T>
	Pixel& operator&=(const T& rhs) {
		c &= rhs;
		return *this;
	}

	template <class T>
	Pixel operator|(const T& rhs) const {
		return Pixel(c | rhs);
	}

	template <class T>
	Pixel& operator|=(const T& rhs) {
		c |= rhs;
		return *this;
	}

	bool operator==(const unsigned& rhs) const { return rhs == c; }
	bool operator==(const Pixel& rhs) const { return rhs.c == c; }

	bool operator!=(const unsigned& rhs) const { return rhs != c; }
	bool operator!=(const Pixel& rhs) const { return rhs.c != c; }


};
static bool operator==(const unsigned& lhs, const Pixel& rhs) { return lhs == rhs.c; }
static bool operator!=(const unsigned& lhs, const Pixel& rhs) { return lhs != rhs.c; }

//typedef unsigned Pixel;

class MapContainer {
	std::vector<Pixel> arr;

public:
	//them being outside is better
	//	outside tampering doesn't interfier with the work of the class
	//	(kinda)
	size_t width, height;

	MapContainer(size_t width = 0, size_t height = 0)
		: width(width), height(height) {
		//arr.reserve(width * height); 
	};

	size_t size() const { return width*height; }

	//	[][]
	// I leave it without protection
	void set(size_t row, size_t col, const Pixel& pixel) {
		arr[col*width + row] = pixel;
	}

	void set(const Point<size_t> p, const Pixel& pixel) {
		this->set(p.x, p.y, pixel);
	}

	const Pixel& get(size_t row, size_t col) const {
		return arr[col*width + row];
	}

	const Pixel& get(const Point<size_t>& p) const {
		return this->get(p.x, p.y);
	}

	Pixel& operator[](size_t i) {
		return arr[i];
	}

	const Pixel& operator[](size_t i) const {
		return arr[i];
	}

	void push_back(const Pixel& p) {
		arr.push_back(p);
	}
	
	void pop_back(const Pixel& p) {
		arr.pop_back();
	}

	void addPath(const Path& path, Pixel c) {

		auto arr = path.getPixelArr();

		for (size_t i = 0; i < arr.size(); ++i) {
			size_t x = arr[i].x;
			size_t y = arr[i].y;

			this->set(x, y, c); // Pink
		}
	}
	
};






#endif







