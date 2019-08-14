

////////// This works //////////////
///int arr[256] = { 0, };		 ///
///(&arr[10])[2] = 3; //ha		 ///
///std::cout << ((&arr[10])[2]); ///
///								 ///
////////////////////////////////////


#include <iostream>
#include <ctime>
#include <string>

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
		map.setPtColor(curr.x, curr.y, PATH_COLOR & pt.color);
		curr = map.clonePtSource(curr.x, curr.y);
	}
	clog << std::endl;
}

int main(int argc, char* argv[]) {

	clog << "Starting Program " << time(NULL) << "\n";

	np::SourceArray map;
	Status s = loadBmp(argv[1], map);
	clog << "	File read status " << s << std::endl;
	if (s != ALL_OK) return s;

	np::Path p = np::solve2(map);

	for (const auto& i : p)
		map.setPtColor(i.x, i.y, PATH_COLOR);

	clog << "	Saving img\n";
	saveBmp(argv[2], map, map.getHeight(), map.getWidth());
	
	return 0;
}

