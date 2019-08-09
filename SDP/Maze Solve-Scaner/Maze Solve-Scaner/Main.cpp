

////////// This works //////////////
///int arr[256] = { 0, };		 ///
///(&arr[10])[2] = 3; //ha		 ///
///std::cout << ((&arr[10])[2]); ///
///								 ///
////////////////////////////////////


#include <iostream>
#include <ctime>

#include "SourceArray.h"
#include "ImageLoader.h"
#include "MazeSolver.h"

#define TESTING_STUFF
using std::clog;
//TODO:: Make logging header with debug define


void drawPath(np::SourceArray& map, const np::Point& pt) {
	clog << "Drawing path to pt (" << pt.x << " , " << pt.y << ")\n  :: ";
	np::Point curr = pt;
	while (map.inBounds(curr)) {
		clog << " (" << curr.x << " , " << curr.y << ")";
		map.setPtColor(curr.x, curr.y, PATH_COLOR);
		curr = map.getPtSource(curr.x, curr.y);
	}
	clog << std::endl;
}

int main(int argc, char* argv[]) {

	clog << "Starting Program " << time(NULL) << "\n";

	np::SourceArray map;
	Status s = loadBmp(argv[1], map);
	if (s != ALL_OK) return s;


	np::ObjContainer identifiedObj = np::fillSourceArrayFromStart(map);

	for (size_t i = 0; i < identifiedObj.size(); ++i) {
		drawPath(map, identifiedObj[i].first);
	}
	//for (np::coord_t x = 0, y = 0;
	//	x < map.getWidth() && y < map.getHeight();
	//	++x, ++y)
	//	map.setPtColor(x, y, PATH_COLOR);

	saveBmp(argv[2], map, map.getHeight(), map.getWidth());

	return 0;
}

