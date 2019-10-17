/*
/// 	First way

Fill the source array from the start, and for every end go back and see what it needs to get to it
This method is kinda efficient towards memory and work time but the result is abismal

///		Second way

Generate a graph by running the fillSourceArrayFromPt function for every object.
Each obj will be characterized by one pt (arbitrary), color and type.
Edges will be between objects (-|).
This is slow and memory hungry but it should yeild better results because
after solving the graph every path should be the shortest (hopefully) or
at least it will show a route that doesn't go to the start every freaking time
it needs sth.

/// 	Third way

A* , heuristic : dist between pts
1. Go through the image and find all relevant pts
2. Find relative centers of those objects
3. Run A* from start to any of the exits (maybe all of them at onece)
4. See if you need to go through doors to get to any of the exits
4.1. If yes look for the keys and get paths
5. Draw path
Find another way..
*/

#include "Logger.h"
#include "MazeSolver.h"

#include <queue>

using namespace np;

typedef std::pair<Point, dist_t> PointDistToExit; //needs a better name
typedef std::pair<Point, ObjType> ObjCenter; 

#define NO_DOORS false

/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Utility functions //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


//
// Used in the BFS
static const unsigned offsetSize = 8;
static const Point offsets[offsetSize] = { 
	Point(-1, 0), //Left
	Point(-1, 1), //Left + Up
	Point(0, 1),  //Up
	Point(1, 1),  //Right + Up
	Point(1, 0),  //Right
	Point(1, -1), //Right + Down
	Point(0, -1), //Down
	Point(-1, -1) //Left + Down
};


static void markVisit(const Point& pt, SourceArray& map) {
	LOG_POINT_MARK_VISIT(pt);
	map.setPtColor(pt.x, pt.y, (map.getPtColor(pt.x, pt.y) & 0x00FFFFFF));
}



static void setPtSource(const Point& pt, const Point& source, SourceArray& map) {
	if (map.inBounds(pt) && map.inBounds(source)) {
		if (map.getPtDistFrStart(pt.x, pt.y) > map.getPtDistFrStart(source.x, source.y) + 1) {
			Point oldS = map.clonePtSource(pt.x, pt.y);
			
			map.setPtSource(pt.x, pt.y, source);
			size_t newDist = map.getPtDistFrStart(source.x, source.y) + 1;
			map.setPtDistFromStart(pt.x, pt.y, newDist);

			LOG_TEXT(newDist); LOG_ENDL;
		}
		else LOG_POINT_CHANGE_SOURCE_FAIL_BETTER_SOURCE(pt, source, map);
	}
	else LOG_POINT_OUT_OF_BOUNDS(source);
}


static bool visited(const Point& pt, SourceArray& map) {
	color_t c = map.getPtColor(pt.x, pt.y) & 0xFF000000;
	return c == 0;
}


static Point checkWidth(const Point& pt, const SourceArray& map) {
	coord_t yTop = pt.y + 1;
	coord_t yBot = pt.y - 1;

	bool reachedTop = false;
	bool reachedBot = false;

	while (	yTop > yBot &&
			(!reachedTop || !reachedBot)) {//contradiction, I know; this needs to be fixed..
		if (yTop < map.getWidth() && sameColor(map.getPtColor(pt.x, yTop), pt.color))
			++yTop;
		else reachedTop = true;
		if (yBot >= 0 && sameColor(map.getPtColor(pt.x, yBot), pt.color))
			--yBot;
		else reachedBot = true;
	}
	--yTop;
	++yBot;

	Point bot;
	if (yTop > yBot && yTop - yBot == KEY_HEIGHT) {
		bot = pt;
		bot.y = yBot;
	}
	return bot;
}

static Point checkHeight(const Point& pt, const SourceArray& map) {
	coord_t xRight = pt.x + 1;
	coord_t xLeft  = pt.x - 1;

	bool reachedLeft = false;
	bool reachedRight = false;

	while (xRight > xLeft &&
		(!reachedRight || !reachedLeft)) {
		if (xRight < map.getHeight() && sameColor(map.getPtColor(xRight, pt.y), pt.color))
			++xRight;
		else reachedRight = true;
		if (xLeft >= 0 && sameColor(map.getPtColor(xLeft, pt.y), pt.color))
			--xLeft;
		else reachedLeft = true;
	}
	--xRight;
	++xLeft;

	Point left;
	if (xRight > xLeft && xRight - xLeft == KEY_WIDTH) {
		left = pt;
		left.x = xLeft;
	}
	return left;
}

static bool isFull(const Point& botLeft, const SourceArray& map) {
	if (!map.inBounds(botLeft)) return false;
	for (coord_t x = botLeft.x; x < botLeft.x + KEY_HEIGHT; ++x)
		for (coord_t y = botLeft.y; y < botLeft.y + KEY_WIDTH; ++y)
			if (!sameColor(map.getPtColor(x, y), botLeft.color))
				return false;
	return true;
}

static bool isSquare(const Point& botLeft, const SourceArray& map) {
	if (!map.inBounds(botLeft)) return false;

	coord_t leftX = botLeft.x - 1;
	coord_t rightX = botLeft.x + KEY_WIDTH + 1;
	coord_t topY = botLeft.y + KEY_HEIGHT + 1;
	coord_t botY = botLeft.y - 1;

	for (coord_t i = botY; i <= topY; ++i) {
		if (map.inBounds(leftX, i) &&
			sameColor(map.getPtColor(leftX, i), botLeft.color))
			return false;
		if (map.inBounds(rightX, i) &&
			sameColor(map.getPtColor(rightX, i), botLeft.color))
			return false;
	}
	
	for (coord_t i = leftX; i <= rightX; ++i) {
		if (map.inBounds(i, topY) && 
			sameColor(map.getPtColor(i, topY), botLeft.color))
			return false;
		if (map.inBounds(i, botY) &&
			sameColor(map.getPtColor(i, botY), botLeft.color))
			return false;
	}

	return true;
}

static ObjType checkShape(const SourceArray& map, const Point& pt){
	
	Point botLeft = checkWidth(pt, map);
	if (!map.inBounds(botLeft)) return Door;

	botLeft = checkHeight(botLeft, map);
	if (!map.inBounds(botLeft)) return Door;

	if (!isFull(botLeft, map) || !isSquare(botLeft, map))
		return Door;

	return Key;
}//here's to hope
	

	
static ObjType identifyObj(const SourceArray& map, const Point& start) {

	color_t objColor = start.color;
	
	if (sameColor(objColor,EXIT_COLOR)) return Exit;
	if (sameColor(objColor, WALL_COLOR)) return Wall;
	if (sameColor(objColor, ENTERENCE_COLOR)) return Enterence;

	return checkShape(map, start);
}

static Point findStart(const SourceArray& map) {
	Point start;
	for(start.x = 0; start.x < map.getHeight(); ++start.x)
		for (start.y = 0; start.y < map.getWidth(); ++start.y) {
			start.color = map.getPtColor(start.x, start.y);
			if ((start.color | 0xFF000000 )== ENTERENCE_COLOR) {
				return start;
			}
		}

	return start;
}

static void visitPoint(	SourceArray* map, 
					const Point* curr, 
					std::queue<Point>* toScanned, 
					std::queue<Point>* borders,
					void (handle*)(SourceArray* map, Point* curr, Point* neighbour, std::queue<Point>* toScan, std::queue<Point>* borders)
				){
			///
	for (size_t i = 0; i < offsetSize; ++i) {
		Point neighbour(curr.x + offsets[i].x , curr.y + offsets[i].y);
	
		if (!map->inBounds(neighbour)) continue;		
			setPtSource(neighbour, *curr, *map);
		
		neighbour.color = map->getPtColor(neighbour.x, neighbour.y);
		
		if(visited(neighbour, *map)) continue;
		markVisit(neighbour, *map);
						
		handle(map, curr, &neighbour, toScan, borders);
	}	
}

void handleBFS_Visit_SameColor(	SourceArray* map, 
								Point* curr, 
								Point* neighbour, 
								std::queue<Point>* toScan, 
								std::queue<Point>* borders,				
							){
					
	// check if it's part of the obj
	if (sameColor(*neighbour, *curr)) {
		LOG_TEXT("		to scan ");
		LOG_POINT_COORD(*neighbour);
		LOG_ENDL;
		toBeScanned->push(*neighbour);
	}
	else {
		LOG_TEXT("		to borders ");
		LOG_POINT_COORD(*neighbour);
		LOG_ENDL;
		borders->push(*neighbour);
	}
}



static void BFS_Visit_SameColor(SourceArray& map, const Point& curr, std::queue<Point>& toBeScanned, std::queue<Point>& borders) {

	LOG_TEXT("	Adding neighbours : \n");
	visitPoint(&map, &curr, &toBeScanned, &border, &handleBFS_Visit_SameColor);//?
}

static void handleAddAdjecent(	SourceArray* map, 
								Point* curr, 
								Point* neighbour, 
								std::queue<Point>* toScan, 
								std::queue<Point>* borders,				
							){
	
	LOG_POINT(neighbour, map);
	LOG_ENDL;
	
	toScan.push(neighbour);
}

static void addAdjacent(SourceArray& map, const Point& pt, std::queue<Point>& toScan){
	
	LOG_TEXT("	Adding adjecent pts :\n");
	visitPoint(&map, &curr, &toBeScanned, nullptr, &handleAddAdjecent);
}

// This is basically BFS with the idea of
//	getting the borders of an object
static void bypassObject(SourceArray& map, const Point& start, std::queue<Point>& toBeScannedBFS) {
	LOG_TEXT(" Bypassing object \n");

	std::queue<Point> toBeScanned;
	toBeScanned.push(start);

	while (!toBeScanned.empty()) {
		Point& curr = toBeScanned.front();

		LOG_TEXT("	Scanning ");
		LOG_POINT(curr, map);
		LOG_ENDL;

		BFS_Visit_SameColor(map, curr, toBeScanned, toBeScannedBFS);
		toBeScanned.pop();
	}
	LOG_TEXT("  Finished bypass\n");

}

static bool passable(color_t color, const std::vector<color_t>& unpassable) {
	if (sameColor(color,WALL_COLOR) )return false;
	for (size_t i = 0; i < unpassable.size(); ++i)
		if (sameColor(color, unpassable[i])) return false;
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// Header functions impl /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//there are going to be several ways to go about this


ObjContainer fillSourceArrayFromPt(SourceArray& map, const Point& start, const std::vector<color_t>& unpassable, std::vector<Point>& unpassDoors, bool includeDoors){
	ObjContainer identifiedObj;
	if (!map.inBounds(start) || start.color == WALL_COLOR) return identifiedObj;

	LOG_TEXT(" Filling source array from pt : ");
	LOG_POINT_COORD(start);
	LOG_ENDL;
	LOG_TEXT((includeDoors ? " Including doors" : " Not including doors"));
	LOG_ENDL;

	//map.setPtDistFromStart(start.x, start.y, 0);
	markVisit(start, map);

	std::queue<Point> toScan;
	if (start.color != CORIDOR_COLOR) {
		bypassObject(map, start, toScan); //Bypassing start //It's more for convinice for the logging
		//Fixing results
		for (size_t i = 0; i < toScan.size(); ++i) {
			map.setPtDistFromStart(toScan.front().x, toScan.front().y, start.distFromStart + 1);
			toScan.push(toScan.front());
			toScan.pop();
		}
	}
	else toScan.push(start);

	while (!toScan.empty()) {
		Point curr = toScan.front();
		toScan.pop();

		LOG_TEXT(" Scanning ");
		LOG_POINT(curr, map);
		LOG_ENDL;

		if (sameColor(curr.color, WALL_COLOR)) continue;
		//markVisit(curr, map);
		LOG_TEXT(" Not wall\n");

		if (sameColor(curr.color,CORIDOR_COLOR)) {
			LOG_TEXT(" Is coridor\n");
			addAdjacent(map, curr, toScan);
		}
		else {
			ObjType t = identifyObj(map, curr);
			LOG_TEXT(" Is ");
			LOG_TEXT((t == Key ? "key" : "door"));
			LOG_ENDL;

			if (t != Door || (includeDoors && passable(curr.color, unpassable))) {//t == Door && inlcude == true
				LOG_ENDL(" Bypassing\n");
				size_t oldQueSize = toScan.size();
				bypassObject(map, curr, toScan);
				//maybe
				if(oldQueSize != toScan.size())
					identifiedObj.push_back(std::make_pair(curr, t));
			}
			else unpassDoors.push_back(curr);
		}

		LOG_TEXT("  Finished scanning pt. Left to scan ");
		LOG_TEXT(toScan.size());
		LOG_ENDL;
	}

	LOG_TEXT("Finished filling\n")

	return identifiedObj;
	//in Th this works
}

ObjContainer np::fillSourceArrayFromPt(SourceArray& map, const Point& start, const std::vector<color_t>& unpassable, bool includeDoors) {
	std::vector<np::Point> unpassDoors;
	return ::fillSourceArrayFromPt(map, start, unpassable, unpassDoors, includeDoors);
}


ObjContainer np::fillSourceArrayFromStart(SourceArray& map, const std::vector<color_t>& unpassable, bool includeDoors) {
	LOG_TEXT("Filling array from enterence point\n");
	Point start = findStart(map);
	map.setPtDistFromStart(start.x, start.y, 0);

	ObjContainer identifiedObj = fillSourceArrayFromPt(map, start, unpassable, includeDoors);

	identifiedObj.push_back(std::make_pair(start, Enterence));
	std::swap(identifiedObj[0], identifiedObj.back());

	return identifiedObj;
}



static bool partOfKey(const np::Point& pt, const np::Point& key) {
	return sameColor(key, pt) &&
		calcDist(pt, key) <= KEY_HEIGHT*KEY_HEIGHT + KEY_WIDTH*KEY_WIDTH;
}

static bool partOfKey(const np::Point& pt, const std::vector<np::Point>& keys) {
	if (sameColor(pt.color, EXIT_COLOR) ||
		sameColor(pt.color, WALL_COLOR) ||
		sameColor(pt.color, ENTERENCE_COLOR) ||
		sameColor(pt.color, CORIDOR_COLOR)) return false;

	for (size_t i = 0; i < keys.size(); ++i) 
		if (partOfKey(pt, keys[i])) return true;
	return false;
}

// revisit
Path getPathTo(const np::Point& to, const SourceArray& map) {
	Path p;
	//I really need to fix that Source array..
	Point curr = to;
	while (map.inBounds(curr)) {
		p.push_back(map.clonePt(curr.x, curr.y));
		curr = map.clonePtSource(curr.x, curr.y);
	}
	return p;
}

/////////////// A* algorithm //////////////////////////////////

// might be a scrapped idea
struct less {
	int operator()(const PointDistToExit& lhs, const PointDistToExit& rhs) {
		return lhs.second < rhs.second;
	}
};

struct greater {
	int operator()(const PointDistToExit& lhs, const PointDistToExit& rhs) {
		return lhs.second > rhs.second;
	}
};

//A*
int np::setPathFromTo(const Point&from, const Point& to, SourceArray& map, const std::vector<color_t>& unpassable) {
	//Validation
	if (!map.inBounds(from) && !map.inBounds(to)) return 1;
	LOG_TEXT("  Starting A*\n");

	markVisit(from, map);
	map.setPtDistFromStart(from.x, from.y, 0);

	std::priority_queue<PointDistToExit, std::vector<PointDistToExit>, greater> toScan;

	toScan.push(std::make_pair(from, calcDist(from, to)));

	bool objectiveFound = false;
	PointDistToExit curr;// = toScan.top();

	while (!toScan.empty()) {
		curr = toScan.top();
		toScan.pop();

		LOG_TEXT(" Scanning top : \n");
		LOG_MAX_HEAP_TOP(curr, map);
		LOG_A_STAR_END_CONDITION(curr, to, map);

		if (curr.second == 0 ||
			sameColor(map.getPtColor(curr.first.x, curr.first.y), map.getPtColor(to.x, to.y))) 
		{
			LOG_TEXT("	FOUND PATH\n");
			setPtSource(to, curr.first, map);
			return 0;
		}

		//Adding adjacent points // I really need to fix that function... !!!!
		const Point& pt = curr.first;
		for (int i = 0; i < offsetSize; ++i) {
			
			LOG_TEXT("	Scanning neighbours\n");

			Point neighbour = pt + offsets[i];
			if (map.inBounds(neighbour) &&
				passable(map.getPtColor(neighbour.x, neighbour.y), unpassable)) 
			{	
				setPtSource(neighbour, pt, map);
				if (!visited(neighbour, map)) {
					markVisit(neighbour, map);
					toScan.push(std::make_pair(neighbour, calcDist(neighbour, to)));
				}
			}
		}

	}

	LOG_TEXT("	NOPE \n");
	return 2;
}


// The Disney-Fox merger
//For now like this
Path getFullPathTo(const Point& to, const std::vector<Point>& keys, SourceArray& map) {
	Path pathToPt = getPathTo(to, map);
	std::vector<const Point*> keysNeeded;

	for (size_t i = 0; i < pathToPt.size() - 1; ++i)		// Go through path //last point should be the start
		if (!sameColor(pathToPt[i].color, CORIDOR_COLOR))	// 
			for(size_t j = 0; j < keys.size(); ++j)			// 
				if(sameColor(pathToPt[i], keys[j]) &&		// if it's a door
					!partOfKey(pathToPt[i], keys[j]))		// get key need to go through
					keysNeeded.push_back(&keys[j]);			//

	std::vector<Path> pathsToKeys;
	for (size_t i = 0; i < keysNeeded.size(); ++i)
		pathsToKeys.push_back(getFullPathTo(*keysNeeded[i], keys, map));

	for (size_t i = 0; i < pathsToKeys.size(); ++i) {
		for (size_t j = 0; j < pathsToKeys[i].size(); ++j)
			pathToPt.push_back(pathsToKeys[i][j]);
	}
	return pathToPt;
}

///////////////////////////////////////////////////////////////////
////////////////////  "Solvers" ///////////////////////////////////
//
//
//	Implements a BFS Scan room by room with updating
//		which rooms are now availabe (?). It works until it 
//		find an exit or scans the reacheble part of the map.
//	  The idea is to make only one BFS scan of the map.
//	  TODO:: Figure out some way to merge paths with min scanning
//
Path np::solve2(np::SourceArray& map) {
	std::vector<np::color_t> unpassable;

	//Scanning first room
	ObjContainer identifiedObj;// = ::fillSourceArrayFromPt(map, start, unpassable, skippedDoors, NO_DOORS);

	//Filters
	std::vector<np::Point> keys;
	std::vector<np::Point> exits;
	std::vector<np::Point> doors;
	
	Point start = findStart(map);
	map.setPtDistFromStart(start.x, start.y, 0);

	std::queue<Point> toScan;
	toScan.push(map.clonePt(start.x, start.y));

	while (!toScan.empty()) {
		Point curr = toScan.front();
		toScan.pop();

		//Scan Room
		identifiedObj = ::fillSourceArrayFromPt(map, curr, unpassable, doors, NO_DOORS);

		//Filter
		for (size_t i = 0; i < identifiedObj.size(); ++i) {
			if (identifiedObj[i].second == Key)  keys.push_back(identifiedObj[i].first);
			else if (identifiedObj[i].second == Exit) exits.push_back(identifiedObj[i].first);
			else if (identifiedObj[i].second == Door) doors.push_back(identifiedObj[i].first);
		}

		if (!exits.empty()) break; 

		//get passable doors
		for (size_t i = 0; i < doors.size(); ++i) {
			for (size_t j = 0; j < keys.size(); ++j)
				if (sameColor(keys[j], doors[i])) { //I feel like I was drunk when I wrote this cicle
					bypassObject(map, doors[i], toScan);
					map.setPtColor(doors[i].x,doors[i].y, 0xFF123456); //Testing
					//removing found door
					std::swap(doors[i], doors.back());
					doors.pop_back();
				}
		}
	}
	
	LOG_KEYS_ARRAY(keys, map);

	if (exits.empty()) return Path();// ;,(

	return getFullPathTo(exits[0], keys, map);
}















