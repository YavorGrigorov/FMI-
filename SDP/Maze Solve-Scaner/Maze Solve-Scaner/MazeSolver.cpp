
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



#include "MazeSolver.h"
#include <iostream>

//I might need a graph class and I should use the last HW
//  to get the path that needs taking....
using namespace np;
using std::clog;


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
	map.setPtColor(pt.x, pt.y, (pt.color & 0x00FFFFFF));
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

static Point checkHeight(const Point& pt, const SourceArray& map) {
	coord_t yTop = pt.y + 1;
	coord_t yBot = pt.y - 1;

	bool reachedTop = false;
	bool reachedBot = false;

	while (	yTop > yBot &&
			yTop < map.getHeight() && yBot >= 0 &&
			!reachedTop && !reachedBot) {

		if (cmpColor(map.getPtColor(pt.x, yTop), pt.color))
			++yTop;
		else reachedTop = true;
		if (cmpColor(map.getPtColor(pt.x, yBot), pt.color))
			--yBot;
		else reachedBot = true;
	}
	--yTop;
	++yBot;

	Point top;
	if (yTop > yBot && yTop - yBot == KEY_HEIGHT) {
		top = pt;
		top.y = yTop;
	}
	return top;
}

static Point checkWidth(const Point& pt, const SourceArray& map) {
	coord_t xRight = pt.x + 1;
	coord_t xLeft  = pt.x - 1;

	bool reachedLeft = false;
	bool reachedRight = false;

	while (xRight > xLeft &&
		xRight < map.getWidth() && xLeft >= 0 &&
		!reachedRight && !reachedLeft) {

		if (cmpColor(map.getPtColor(xRight, pt.y), pt.color))
			++xRight;
		else reachedRight = true;
		if (cmpColor(map.getPtColor(xLeft, pt.y), pt.color))
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

static bool isFull(const Point& topLeft, const SourceArray& map) {
	for (coord_t x = topLeft.x; x < map.getWidth(); ++x)
		for (coord_t y = topLeft.y; y < map.getHeight(); ++y)
			if (!map.inBounds(x, y) || cmpColor(map.getPtColor(x, y), topLeft.color))
				return false;
	return true;
}

//ugly but, that's what i can think of right now
static bool isSquare(const Point& topLeft, const SourceArray& map) {
	coord_t leftX = topLeft.x - 1;
	coord_t rightX = topLeft.x + KEY_WIDTH + 1;
	coord_t topY = topLeft.y + 1;
	coord_t botY = topLeft.y - KEY_HEIGHT - 1;

	for (coord_t i = botY; i <= topY; ++i) {
		if (map.inBounds(leftX, i) &&
			!cmpColor(map.getPtColor(leftX, i), topLeft.color))
			return false;
		if (map.inBounds(rightX, i) &&
			!cmpColor(map.getPtColor(rightX, i), topLeft.color))
			return false;
	}
	
	for (coord_t i = leftX; i <= rightX; ++i) {
		if (map.inBounds(i, topY) && 
			!cmpColor(map.getPtColor(i, topY), topLeft.color))
			return false;
		if (map.inBounds(i, botY) &&
			!cmpColor(map.getPtColor(i, botY), topLeft.color))
			return false;
	}

	return true;
}

static ObjType checkShape(const SourceArray& map, Point& pt){
	
	Point topLeft = checkHeight(pt, map);
	if (!map.inBounds(topLeft)) return Door;
	topLeft = checkWidth(topLeft, map);
	if (!map.inBounds(topLeft)) return Door;

	if (!isFull(topLeft, map) || !isSquare(topLeft, map))
		return Door;

	return Key;
}//here's to hope
	

	
static ObjType identifyObj(const SourceArray& map, const Point& start) {

	Point topLeft = start;
	color_t objColor = topLeft.color;
	if ((objColor | 0xFF000000) == EXIT_COLOR) return Exit;
	if ((objColor | 0xFF000000) == WALL_COLOR) return Wall;

	return checkShape(map, topLeft);
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
static void BFS_Visit_SameColor(SourceArray& map, const Point& curr, const Point& source, std::queue<Point>& toBeScanned, std::queue<Point>& borders) {
	if (visited(curr, map)) return;
	clog << " Point not visited\n";

	clog << " Adding neighbours : ";
	for (size_t i = 0; i < offsetSize; ++i) {
		Point neighbour(curr.x + offsets[i].x , curr.y + offsets[i].y);
		if(!map.inBounds(neighbour) || visited(neighbour, map)) continue;
		
		//all points in the object and it's outer border will have the same source
		map.setPtSource(neighbour.x, neighbour.y, source);
		neighbour.color = map.getPtColor(neighbour.x, neighbour.y);
		
		// check if it's part of the obj
		if (sameColor(neighbour, curr)) {
			clog << "\n to scan (" << neighbour.x << " , " << neighbour.y << ")";
			toBeScanned.push(neighbour);
		}
		else {
			clog << "\n to borders (" << neighbour.x << " , " << neighbour.y << ")";
			borders.push(neighbour);
		}
	}
	
	markVisit(curr, map); 
}

// This is basically BFS with the idea of
//	getting the borders of an object
static void bypassObject(SourceArray& map, const Point& start, std::queue<Point>& toBeScannedBFS) {
	clog << " Bypassing object \n";

	std::queue<Point> toBeScanned;
	toBeScanned.push(start);

	while (!toBeScanned.empty()) {
		Point& curr = toBeScanned.front();
		
		clog << " Scanning pt (" << curr.x << " , " << curr.y << ")\n";
		BFS_Visit_SameColor(map, curr, start, toBeScanned, toBeScannedBFS);

		toBeScanned.pop();
	}
}



void addAdjacent(SourceArray& map, const Point& pt, std::queue<Point>& toScan){
	clog << " Adding adjecent pts :";
	for(int i = 0; i < offsetSize; ++i){
		Point neighbour = pt + offsets[i];
		if (map.inBounds(neighbour) && !visited(neighbour, map)) {
			neighbour.color = map.getPtColor(neighbour.x, neighbour.y);

			clog << "\n(" << neighbour.x << " , " << neighbour.y << ") color "
				<< std::hex << neighbour.color << std::dec;
			
			map.setPtSource(neighbour.x, neighbour.y, pt);
			toScan.push(neighbour);
		}
	}
	clog << std::endl;
}




//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// Header functions impl /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//there are going to be several ways to go about this



ObjContainer np::fillSourceArrayFromPt(SourceArray& map, const Point& start, bool includeDoors){
	
	clog << " Filling source array from pt : (" << start.x << " , " << start.y << ")\n";
	clog << (includeDoors ? " Including doors" : " Not including doors") << std::endl;

	std::queue<Point> toScan;
	bypassObject(map, start, toScan);

	//testing
	ObjContainer identifiedObj;

	while(!toScan.empty()){
		Point curr = toScan.front();
		
		clog << " Scanning pt (" << curr.x << " , " << curr.y << ") color "
			<< std::hex << curr.color  << std::dec << "\n";

		if(!visited(curr, map)){ //if all goes well this should always be true
			clog << " Not visited\n";
			
			if (curr.color == CORIDOR_COLOR) {
				clog << " pt is part of coridor\n";
				addAdjacent(map, curr, toScan);
			}
			else if (curr.color != WALL_COLOR) {
				ObjType t = identifyObj(map, curr); // to be worked on .. ALOt
				
				clog << " pt is " << (t == Key ? "key" : "door") << std::endl;

				if (t != Door || includeDoors) {
					bypassObject(map, curr, toScan);
					
					identifiedObj.push_back(std::make_pair(curr, t));
				}
			}
			
			markVisit(curr, map);
		}
		clog << " Removing pt from queue\n";
		toScan.pop();
	}

	clog << "Finished filling\n";

	return identifiedObj;
	//in Th this works
}

ObjContainer np::fillSourceArrayFromStart(SourceArray& map, bool includeDoors) {
	clog << "Filling array from enterence point\n";
	Point start = findStart(map);
	return fillSourceArrayFromPt(map, start, includeDoors);
}




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













