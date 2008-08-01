#ifndef COORDINATE_H
/* defines */
#define COORDINATE_H

/* forward declare */
class Coordinate;

/* includes */
#include "Point.h"

/* namespace */
using namespace std;

/* Coordinate */
class Coordinate : public Point {
	public:
		/* variables */
		int level;

		/* constructors */
		Coordinate(int level = 0, int row = 0, int col = 0);
		Coordinate(int level, const Point &point = Point());

		/* operator overloading */
		bool operator<(const Coordinate &coordinate) const;
		bool operator>(const Coordinate &coordinate) const;
		bool operator<=(const Coordinate &coordinate) const;
		bool operator>=(const Coordinate &coordinate) const;
		bool operator==(const Coordinate &coordinate) const;
		bool operator!=(const Coordinate &coordinate) const;
};
#endif
