
/*
 Test what we have now so we see what's brocken!!!!!!
	Done and it is everything...

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


///////////////////////////////////////////////
/////// Question: Does this work?  ////////////
///#define cnt 1                           	///
///#define cnt_ cnt++                      	///
///int f(){                                	///
///	std::cout << cnt_ 	//out : 1       	///
///			<< cnt_		//out : 2       	///
///			<< cnt_;	//out : 3       	///
///}                                       	///
///////////////////////////////////////////////
///////////////////////////////////////////////

//Another header
// Needs more work
#define LOG_MAP map 
#define LOG_POINT(Pt) clog	<< " ( " << (Pt).x << " , " << (Pt).y << ") color "  \
							<< std::hex << LOG_MAP.getPtColor(Pt.x, Pt.y)  << std::dec \
					<< " dist to Start " << LOG_MAP.getPtDistFrStart(Pt.x, Pt.y) << std::endl
#define LOG_MAX_HEAP_TOP(T) LOG_POINT(T.first); clog << "	dist to Objective " << T.second << std::endl
#define LOG_A_STAR_END_CONDITION(T , Pt ,Map) clog	<< T.second << " == 0 || " \
													<< std::hex \
													<< Map.getPtColor(T.first.x, T.first.y) \
													<< " == " << Map.getPtColor(Pt.x, Pt.y) \
													<< std::dec \
													<< std::endl




#include "MazeSolver.h"
#include <iostream>

#include <queue>
#include <unordered_map> //See if you need this
//I might need a graph class and I should use the last HW
//  to get the path that needs taking....
using namespace np;
using std::clog;

typedef std::pair<Point, dist_t> PointDistToExit; //needs a better name
typedef std::pair<Point, ObjType> ObjCenter; //this idea is dead, for now


#define NO_DOORS false

template<>
struct std::hash<np::Point> {
	size_t operator()(const np::Point& p) {
		return std::hash<coord_t>{}(p.x + p.y);
	}
};
typedef std::unordered_map<np::Point, Path> EdgeArr;
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
	clog << " Marking pt (" << pt.x << " , " << pt.y << ") as visited\n";
	map.setPtColor(pt.x, pt.y, (map.getPtColor(pt.x, pt.y) & 0x00FFFFFF));
}

//
// This function made this program eat only O(n) memory 
///		(n is the size of the pic) // more precisely 3*n
//	for a pick 74KB it used to eat alot of MBs, like 100ths
//	;d
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

// BFS_VISIT_SAMECOLOR and AddAdjecent are basically the same
//	TODO::fix
static void BFS_Visit_SameColor(SourceArray& map, const Point& curr, std::queue<Point>& toBeScanned, std::queue<Point>& borders) {
	//if (visited(curr, map)) return;
	//markVisit(curr, map);
	//clog << "	Point not visited\n";

	clog << "	Adding neighbours : \n";
	for (size_t i = 0; i < offsetSize; ++i) {
		Point neighbour(curr.x + offsets[i].x , curr.y + offsets[i].y);
		if (!map.inBounds(neighbour)) continue;
		
		map.setPtSource(neighbour.x, neighbour.y, curr);
		neighbour.color = map.getPtColor(neighbour.x, neighbour.y);
		
		if(visited(neighbour, map)) continue;
		markVisit(neighbour, map);
		// check if it's part of the obj
		if (sameColor(neighbour, curr)) {
			clog << "		to scan (" << neighbour.x << " , " << neighbour.y << ")\n";
			toBeScanned.push(neighbour);
		}
		else {
			clog << "		to borders (" << neighbour.x << " , " << neighbour.y << ")\n";
			borders.push(neighbour);
		}
	}
	
}



// This is basically BFS with the idea of
//	getting the borders of an object
static void bypassObject(SourceArray& map, const Point& start, std::queue<Point>& toBeScannedBFS) {
	clog << " Bypassing object \n";

	std::queue<Point> toBeScanned;
	toBeScanned.push(start);

	while (!toBeScanned.empty()) {
		Point& curr = toBeScanned.front();

		clog << "	Scanning ";
		LOG_POINT(curr);
		clog << std::endl;

		BFS_Visit_SameColor(map, curr, toBeScanned, toBeScannedBFS);
		toBeScanned.pop();
	}
	clog << "  Finished bypass\n";

}

// TODO:: template wifht handle
void addAdjacent(SourceArray& map, const Point& pt, std::queue<Point>& toScan){
	clog << "	Adding adjecent pts :\n";

	for(int i = 0; i < offsetSize; ++i){
		Point neighbour = pt + offsets[i];
		
		if (!map.inBounds(neighbour)) continue;
		map.setPtSource(neighbour.x, neighbour.y, pt);
			
		neighbour.color = map.getPtColor(neighbour.x, neighbour.y);
		if(!visited(neighbour, map)) {
			markVisit(neighbour, map);
			LOG_POINT(neighbour);
			toScan.push(neighbour);
		}
	}

	clog << std::endl;
}


bool passable(color_t color, const std::vector<color_t>& unpassable) {
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

	clog << " Filling source array from pt : (" << start.x << " , " << start.y << ")\n";
	clog << (includeDoors ? " Including doors" : " Not including doors") << std::endl;

	//map.setPtDistFromStart(start.x, start.y, 0);
	markVisit(start, map);

	std::queue<Point> toScan;
	if(start.color != CORIDOR_COLOR)
		bypassObject(map, start, toScan); //Bypassing start //It's more for convinice for the logging
	//Fixing results
	for (size_t i = 0; i < toScan.size(); ++i) {
		map.setPtDistFromStart(toScan.front().x, toScan.front().y, start.distFromStart + 1);
		toScan.push(toScan.front());
		toScan.pop();
	}

	while (!toScan.empty()) {
		Point curr = toScan.front();
		toScan.pop();

		clog << " Scanning ";
		LOG_POINT(curr);
		clog << std::endl;

		if (sameColor(curr.color, WALL_COLOR)) continue;
		//markVisit(curr, map);
		clog << " Not wall\n";

		if (sameColor(curr.color,CORIDOR_COLOR)) {
			clog << " Is coridor\n";
			addAdjacent(map, curr, toScan);
		}
		else {
			ObjType t = identifyObj(map, curr);
			clog << " Is " << (t == Key ? "key" : "door") << std::endl;

			if (t != Door || (includeDoors && passable(curr.color, unpassable))) {//t == Door && inlcude == true
				clog << " Bypassing\n";
				size_t oldQueSize = toScan.size();
				bypassObject(map, curr, toScan);
				//maybe
				if(oldQueSize != toScan.size())
					identifiedObj.push_back(std::make_pair(curr, t));
			}
			else unpassDoors.push_back(curr);
		}
		clog << "  Finished scanning pt. Left to scan " << toScan.size() << std::endl;
	}

	clog << "Finished filling\n";

	return identifiedObj;
	//in Th this works
}

ObjContainer np::fillSourceArrayFromPt(SourceArray& map, const Point& start, const std::vector<color_t>& unpassable, bool includeDoors) {
	std::vector<np::Point> unpassDoors;
	return ::fillSourceArrayFromPt(map, start, unpassable, unpassDoors, includeDoors);
}


ObjContainer np::fillSourceArrayFromStart(SourceArray& map, const std::vector<color_t>& unpassable, bool includeDoors) {
	clog << "Filling array from enterence point\n";
	Point start = findStart(map);
	map.setPtDistFromStart(start.x, start.y, 0);

	ObjContainer identifiedObj = fillSourceArrayFromPt(map, start, unpassable, includeDoors);

	identifiedObj.push_back(std::make_pair(start, Enterence));
	std::swap(identifiedObj[0], identifiedObj.back());

	return identifiedObj;
}


dist_t calcDist(const Point& fr, const Point& to) {
	coord_t x = (to.x - fr.x)*(to.x - fr.x);
	coord_t y = (to.y - fr.y)*(to.y - fr.y);
	return x + y;
}

bool partOfKey(const np::Point& pt, const std::vector<const np::Point*>& keys) {
	if (sameColor(pt.color, EXIT_COLOR) ||
		sameColor(pt.color, WALL_COLOR) ||
		sameColor(pt.color, ENTERENCE_COLOR) ||
		sameColor(pt.color, CORIDOR_COLOR)) return false;

	for (size_t i = 0; i < keys.size(); ++i) {
		if (sameColor(*keys[i], pt))
			if (calcDist(pt, *keys[i]) <= KEY_HEIGHT*KEY_HEIGHT + KEY_WIDTH*KEY_WIDTH) return true;
	}
	return false;
}


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

/////////////// A* algorithm //////////////////////////////////


//PointDistToExit getClosestObjectiveDist(const PointDistToExit& pt, const std::vector<const Point*>& objectives) {
//	PointDistToExit res = pt;
//
//	dist_t minDist = MAX_DIST;
//	for (size_t i = 0; i < objectives.size(); ++i) {
//		dist_t currDist = calcDist(pt.first, *objectives[i]);
//		if (currDist < minDist) minDist = currDist;
//	}
//
//	res.second = minDist;
//	return res;
//}


//A*
int np::setPathFromTo(const Point&from, const Point& to, SourceArray& map, const std::vector<color_t>& unpassable) {
	//Validation
	if (!map.inBounds(from) && !map.inBounds(to)) return 1;
	clog << "  Starting A*\n";

	markVisit(from, map);
	map.setPtDistFromStart(from.x, from.y, 0);

	std::priority_queue<PointDistToExit, std::vector<PointDistToExit>, greater> toScan;

	toScan.push(std::make_pair(from, calcDist(from, to)));

	bool objectiveFound = false;
	PointDistToExit curr;// = toScan.top();

	while (!toScan.empty()) {
		curr = toScan.top();
		toScan.pop();

		clog << " Scanning top : \n";
		LOG_MAX_HEAP_TOP(curr);
		LOG_A_STAR_END_CONDITION(curr, to, map);

		if (curr.second == 0 ||
			sameColor(map.getPtColor(curr.first.x, curr.first.y), map.getPtColor(to.x, to.y))) 
		{
			clog << "	FOUND PATH\n";
			map.setPtSource(to.x, to.y, curr.first);
			return 0;
		}

		//Adding adjacent points // I really need to fix that function... !!!!
		const Point& pt = curr.first;
		for (int i = 0; i < offsetSize; ++i) {
			
			clog << "	Scanning neighbours\n";

			Point neighbour = pt + offsets[i];
			if (map.inBounds(neighbour) &&
				passable(map.getPtColor(neighbour.x, neighbour.y), unpassable)) 
			{	
				map.setPtSource(neighbour.x, neighbour.y, pt);
				if (!visited(neighbour, map)) {
					markVisit(neighbour, map);
					toScan.push(std::make_pair(neighbour, calcDist(neighbour, to)));
				}
			}
		}

	}

	clog << "	NOPE \n";
	return 2;
}



///////////////////////////////////
/////  "Solvers" //////////////////

//
//
//	Implements a BFS Scan room by room with updating
//		which rooms are now availabe (?). It works until it 
//		find an exit or scans the reacheble part of the map.
//	  The idea is to make only one BFS scan of the map.
//	  TODO:: Figure out some way to merge paths with min scanning
//	Also, make it more presentable...
//
Path np::solve2(np::SourceArray& map) {
	std::vector<np::color_t> unpassable;
	std::vector<Point> skippedDoors;

	//Scanning first room
	ObjContainer identifiedObj;// = ::fillSourceArrayFromPt(map, start, unpassable, skippedDoors, NO_DOORS);

	//Filters
	std::vector<const np::Point*> keys;
	std::vector<const np::Point*> exits;
	std::vector<const np::Point*> doors;
	size_t filteredObj = 0;
	
	Point start = findStart(map);
	map.setPtDistFromStart(start.x, start.y, 0);

	std::queue<Point> toScan;
	toScan.push(map.clonePt(start.x, start.y));

	while (!toScan.empty()) {
		Point curr = toScan.front();
		toScan.pop();
		//Scan Room
		identifiedObj = ::fillSourceArrayFromPt(map, curr, unpassable, skippedDoors, NO_DOORS);
		//if (identifiedObj.empty()) return Path();

		//Filter
		for (size_t& i = filteredObj; i < identifiedObj.size(); ++i) {
			if (identifiedObj[i].second == Key)  keys.push_back(&identifiedObj[i].first);
			else if (identifiedObj[i].second == Exit) exits.push_back(&identifiedObj[i].first);
			else if (identifiedObj[i].second == Door) doors.push_back(&identifiedObj[i].first);
		}

		if (!exits.empty()) break; 

		std::vector<size_t> doorsToPass;
		//mark keylsess doors
		for (size_t i = 0; i < doors.size(); ++i) {
			//bool dontRemove = false;
			for (size_t j = 0; j < keys.size(); ++j)
				if (sameColor(*keys[j], skippedDoors[j])) {
					doorsToPass.push_back(j);
				//	dontRemove = true;
				}
			//if (!dontRemove) unpassable.push_back(doors[i]->color);
		}

		// needs some way to compress these
		for (auto i : doorsToPass) //if all's well, it will ad only the other side
			bypassObject(map, skippedDoors[i], toScan);

	}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


	if (exits.empty()) return Path();// ;,(

	std::vector<color_t> keysNeeded;
	Path pToE = getPathTo(*exits[0], map);
	for (const auto& i : pToE)
		if (!sameColor(i.color, CORIDOR_COLOR) && !partOfKey(i, keys))
			keysNeeded.push_back(i.color);
	
	return pToE;
	//Oh fuck..
	//Keys could be in diff rooms

	// For now Stupid Solve


	// And now the fucked up part
	//Currently   E -> Dn -> Dn-1 -> .. -> D1 -> S
	// Have to modify path as follows
	//	S-> K1 [..]-> K2 [..] -> K3 [..]-> .. [..]-> E
}


//
//Path solveStupid(np::SourceArray&map) {
//	std::vector<np::color_t> unpassable;
//	ObjContainer identifiedObj = fillSourceArrayFromStart(map,unpassable);
//
//	np::Point start = identifiedObj[0].first;
//	std::vector<const np::Point*> keys;
//	std::vector<const np::Point*> exits;
//	std::vector<const np::Point*> doors;
//	//Filter
//	for (size_t i = 0; i < identifiedObj.size(); ++i) {
//		if (identifiedObj[i].second == Key)  keys.push_back(&identifiedObj[i].first);
//		else if (identifiedObj[i].second == Exit) exits.push_back(&identifiedObj[i].first);
//		else if (identifiedObj[i].second == Door) doors.push_back(&identifiedObj[i].first);
//	}
//	if (exits.empty()) return Path();
//	//remove keylsess doors
//	for (size_t i = 0; i < doors.size(); ++i) {
//		bool dontRemove = false;
//		for (size_t j = 0; j < keys.size(); ++j)
//			if (sameColor(*keys[j],*doors[j])) dontRemove = true;
//		if (!dontRemove) unpassable.push_back(doors[i]->color);
//	}
//	
//	if (!unpassable.empty()) {
//		map.reset();
//		fillSourceArrayFromPt(map, start, unpassable);
//	}
//
//	std::vector<Path> pathsToExits;
//	for (size_t i = 0; i < exits.size(); ++i) {
//		pathsToExits.push_back(getPathTo(*exits[i], map));
//	}
//
//	Path* shortest = &pathsToExits[0];
//	for (size_t i = 0; i < pathsToExits.size(); ++i)
//		if (pathsToExits[i].size() > 1 &&
//			pathsToExits[i].size() < shortest->size())
//				shortest = &pathsToExits[i];
//
//	Path& path = *shortest; //Exit -> Start
//	ObjContainer inTheWay;
//	for (size_t i = 1; i < path.size(); ++i) {
//		if (sameColor(path[i], CORIDOR_COLOR)) continue;
//		if (partOfKey(path[i], keys)) //there's a bug when a door and a key are really close to eachother
//			inTheWay.push_back(std::make_pair(path[i], Key));
//		else {
//			inTheWay.push_back(std::make_pair(path[i], Door));
//			color_t doorCol = path[i].color;
//			//Skip door
//			while (path[i + 1].color == doorCol) ++i;
//		}
//	}
//	std::vector<color_t> accessToKey;
//	std::vector<std::pair<Point, color_t>> needKeyAt;
//	for (size_t i = inTheWay.size() - 1; i > 0; ++i) {
//		if (inTheWay[i].second == Key) accessToKey.push_back(inTheWay[i].first.color);
//		else {
//			bool needKey = true;
//			Point& pt = inTheWay[i].first;
//			for (size_t j = 0; j < accessToKey.size(); ++i)
//				if (accessToKey[j] == pt.color) needKey = false;
//			if (needKey) needKeyAt.push_back(std::make_pair(pt, pt.color));
//		}
//	}
//
//	for (size_t i = 0; i < needKeyAt.size(); ++i) {
//		
//	}
//}
//

//int np::setPathFromStartToAnyExit(SourceArray & map) {
//	Point start = findStart(map);
//	
//	std::vector<const Point*> objects = getObjects(map);
//
//	return 1;
//}


/*
int np::setPathFromTo(const Point&from, const Point& to, SourceArray& map, const std::vector<color_t>& unpassable) {
//Validation
if (!map.inBounds(from) && !map.inBounds(to)) return 1;
clog << "  Starting A*\n";

markVisit(from, map);
map.setPtDistFromStart(from.x, from.y, 0);

std::priority_queue<PointDistToExit, std::vector<PointDistToExit>, greater> toScan;

toScan.push(std::make_pair(from, calcDist(from, to)));

bool objectiveFound = false;
PointDistToExit curr;// = toScan.top();

while (!toScan.empty()) {
curr = toScan.top();
toScan.pop();

clog << " Scanning top : \n";
LOG_MAX_HEAP_TOP(curr);
LOG_A_STAR_END_CONDITION(curr, to, map);

if (curr.second == 0 ||
sameColor(map.getPtColor(curr.first.x, curr.first.y), map.getPtColor(to.x, to.y)))
{
clog << "	FOUND PATH\n";
map.setPtSource(to.x, to.y, curr.first);
return 0;
}

//Adding adjacent points // I really need to fix that function... !!!!
const Point& pt = curr.first;
for (int i = 0; i < offsetSize; ++i) {

clog << "	Scanning neighbours\n";

Point neighbour = pt + offsets[i];
if (map.inBounds(neighbour)) {
neighbour.color = map.getPtColor(neighbour.x, neighbour.y);
std::queue<np::Point> border;

if (passable(neighbour.color, unpassable) && neighbour.color != CORIDOR_COLOR)
bypassObject(map, neighbour, border);

while (!border.empty()) {
np::Point borderCurr = border.front();
map.setPtSource(neighbour.x, neighbour.y, pt);

if (!visited(neighbour, map)) {
markVisit(neighbour, map);
toScan.push(std::make_pair(neighbour, calcDist(neighbour, to)));

}
}
}
}

}

clog << "	NOPE \n";
return 2;
}

*/



//
/// To look at (maybe in shame)
/*

static ObjType getXBoundByY(const SourceArray& map, color_t color, coord_t& x,
const coord_t up, const coord_t down,
void (*next)(coord_t&),
bool (*valid)(coord_t, const SourceArray& map))
{

bool topBorder, botBorder, innerHeightBorder;
do {
topBorder = map.getPtColor(x, up - 1) == color;
botBorder = map.getPtColor(x, down + 1) == color;
innerHeightBorder = (topBorder && botBorder) || (!topBorder && !botBorder); // <=>

bool outerHeightBorder = map.getPtColor(x, up) != color &&
map.getPtColor(x, down) != color;

if(!innerHeightBorder || !outerHeightBorder)
return Door;
next(x);
} while (valid(x, map) && topBorder);

return Key;
}

*/













