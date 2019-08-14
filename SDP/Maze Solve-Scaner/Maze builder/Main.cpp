
#include "C:\Users\Yavka\source\repos\FMI\FMI GitHub\FMI-\SDP\Maze Solve-Scaner\Maze Solve-Scaner\ImageLoader.h"
#include "C:\Users\Yavka\source\repos\FMI\FMI GitHub\FMI-\SDP\Maze Solve-Scaner\Maze Solve-Scaner\MazeRules.h"
#include "vector"
#include <random>
#include <ctime>
#include <iostream>

using namespace np;
typedef SourceArray MapContainer;
typedef std::vector<Point> ObjContainer;

#define MAGIC_NUMBER 20
#define TEST_KEY_CNT 3
#define TEST_DOOR_CNT TEST_KEY_CNT + rand() % MAGIC_NUMBER
#define TEST_EXIT_CNT 4
#define TEST_WALL_CNT 5

// Utility

bool inRange(coord_t x, coord_t left, coord_t right) {
	return x >= left && x <= right;
}

size_t arrPos(coord_t x, coord_t y, size_t width) {
	return y*width + x;
}

void removePt(ObjContainer& arr, size_t idx) {
	for (size_t i = idx; i < arr.size() - 1; ++i) {
		arr[i] = arr[i + 1];
	}
	arr.pop_back();
}

//

// Generators
color_t genColor() {
	color_t color = 0;

	do {
		color = 0xFF000000 |
			((rand() % 256) << 16) |
			((rand() % 256) << 8) |
			(rand() % 256);
	} while (	color == ENTERENCE_COLOR &&
				color == EXIT_COLOR	&&
				color == WALL_COLOR);
	return color;
}

Point pickPtFromArr(std::vector<Point>& arr, color_t color, const MapContainer& map) {
	const size_t dirrSize = 8;
	const Point dirrections[dirrSize] = { Point(1,0), Point(1,1), Point(0,1),
		Point(-1,1), Point(-1,0), Point(-1, -1),
		Point(0,-1), Point(1,-1) };

	
	Point pt;
	bool innerPt = true;
	while (innerPt) {
		//select a point
		size_t ptIdx = rand() % arr.size();
		pt = arr[ptIdx];
		//check if it is an inner pt
		// Imma fucking idiot...... (forgot to look to the sides....)
		for (size_t s = 0; s < dirrSize; ++s) {
			Point side(pt.x + dirrections[s].x, pt.y + dirrections[s].y);
			side.color = map.getPtColor(side.x, side.y);
			if (side.color != color) {
				innerPt = false;
				break;
			}
		}
		if (innerPt) removePt(arr, ptIdx);
		//if not continue
	}
	pt.color = color;
	return pt;
}

Point pickDirr(const Point& pt, const MapContainer& map) {
	const size_t dirrSize = 8;
	const Point dirrections[dirrSize] = {	Point(1,0), Point(1,1), Point(0,1),
											Point(-1,1), Point(-1,0), Point(-1, -1),
											Point(0,-1), Point(1,-1) };

	size_t dirr = 0;
	
	Point curr = pt;

	do {
		dirr = rand() % dirrSize;
		curr.x = dirrections[dirr].x + pt.x;
		curr.y = dirrections[dirr].y + pt.y;
	} while (	!inRange(curr.x, 0, map.getWidth()) ||
				!inRange(curr.y, 0, map.getHeight()) ||
				map.getPtColor(curr.x, curr.y) == pt.color);

	curr.color = map.getPtColor(curr.x, curr.y);
	return curr;

}

Point genRandObj(MapContainer& map, size_t width, size_t height, color_t color) {
	size_t objPixelCnt = rand() % (width * height / 3);
	std::vector<Point> objPts;
	objPts.reserve(objPixelCnt);

	Point start((rand() % width), (rand() % height), color);
	map.setPtColor(start.x, start.y, color);
	objPts.push_back(start);
	
	std::cout	<< " Color = "<< color 
				<< "\n Map Width = " << width 
				<< " Height = " << height << "\n iterator ::\n";


	for (size_t i = 1; i < objPixelCnt; ++i) {
		Point pt = pickPtFromArr(objPts, color, map);

		std::cout << i << " ";

		pt = pickDirr(pt, map);
		objPts.push_back(pt);
	}
	std::cout << std::endl;

	return start;
}

void genStart(MapContainer& map, size_t width, size_t height, ObjContainer& objs) {
	Point start = genRandObj(map, width, height, ENTERENCE_COLOR);
	objs.push_back(Point(start.x, start.y, ENTERENCE_COLOR));
}


//TO_FINISH::
unsigned genKeys(MapContainer& map, size_t width, size_t height, ObjContainer& objs) {
	unsigned keyCnt = TEST_KEY_CNT;//MAGIC_NUMBER;//rand();

	for (unsigned i = 0; i < keyCnt; ++i) {
		//TODO::
		objs.push_back(Point(-1, -1, genColor()));
	}
	return keyCnt;
}


void genDoors(MapContainer& map, size_t width, size_t height, ObjContainer& objs) {
	unsigned doorCnt = TEST_DOOR_CNT;//rand() % (2 * objs.size());

	for (size_t i = 1; i < doorCnt; ++i)
		genRandObj(map, width, height, objs[i].color);

}

void genWalls(MapContainer& map, size_t width, size_t height) {
	size_t wallCnt = TEST_WALL_CNT;//rand() % MAGIC_NUMBER + 1;

	std::cout << " Generating Walls ::\n";
	for (size_t i = 0; i < wallCnt; ++i) {
		std::cout << " Wall " << i << "::\n";
		genRandObj(map, width, height, WALL_COLOR);
	}
}

void genExits(MapContainer& map, size_t width, size_t height, ObjContainer& objs) {
	unsigned exitCnt = TEST_EXIT_CNT;//rand() % MAGIC_NUMBER + 1;

	std::cout << "Generating Exits ::\n";
		for (size_t i = 0; i < exitCnt; ++i) {
			std::cout << "Exit " << i << " : ";
			genRandObj(map, width, height, EXIT_COLOR);
		}
}
//
// It compiles buuut
//	it might ne going into an infinite loop or sth...
//	R.I.P.
int main() {

	srand(time(NULL));
	size_t width = rand() % 1024 + 50;
	size_t height = rand() % 1024 + 50;

	MapContainer map(width, height);
	ObjContainer objs;

	genWalls(map, width, height);
	genStart(map, width, height, objs);
	genKeys(map, width, height, objs);
	genDoors(map, width, height, objs);

	saveBmp("generatedMap.bmp", map, height, width);

	return 0;
}






