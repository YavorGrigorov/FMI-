


typedef unsigned T;

class Vector {
	typedef Point<T> pt;
	pt from, to;

public:
	Vector() = default;
	Vector(const pt& from, const pt& to) : from(from), to(to) {};
	Vector(const T& x1, const T& y1, const T& x2, const T& y2)
		: from(x1, y1), to(x2, y2) {};

	pt& getFrom() { return from; }
	const pt& getFrom() const { return to; }

	pt& getTo() { return to; }
	const pt& getTo() const { return to; }

	T length() const;

	//typedef double Scalar;
	////template <class Scalar>
	//Vector& operator*=(const Scalar& scalar) {
	//	x *= scalar;
	//	y *= scalar;
	//	return *this;
	//}
	//
	//typedef double Scalar;
	////template <class Scalar>
	//Vector operator*(const Scalar& scalar) {
	//	Vector temp(*this);
	//	temp *= scalar;
	//	return temp;
	//}

	bool contains(const Point<T>& pt) const;

#include <vector> 
	typedef std::vector<pt> Container;

private:
	void getPixels(Container* container);

};