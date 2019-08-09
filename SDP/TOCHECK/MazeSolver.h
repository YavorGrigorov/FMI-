

#ifndef MAZE_SOLVER_INCLUDED__
#define MAZE_SOLVER_INCLUDED__

#include "Objects.h"
#include "MazeRules.h"
#include <vector>
#include <queue>

//
// (0,0) is in the bot left
//
//
template <class A, class B>
struct Pair {
	A first;
	B second;

	Pair() = default;
	Pair(const A& first, const B& second)
		:first(first), second(second) {}
};

enum ObjType {
	Key, Door, Exit, Wall
};

typedef Point<size_t> pt;
typedef Pair<pt, Path> pathToPt;

enum Side {
	left = 8,
	up = 4,
	right = 2,
	down = 1
};
static const Point<int> offsets[4] = { { -1, 0 },{ 0, -1 },{ 1, 0 },{ 0, 1 } };

ObjType identifyObj(const MapContainer& map, const pt& start) {

	Pixel objColor = map.get(start.x, start.y);
	if (objColor == ExitColor) return Exit;
	if (objColor == WallColor) return Wall;

	//It's for the fullness check
	Point<size_t> topLeft = { start.x, start.y };

	//Check Height
	size_t objHeight = 0;

	for (int up = start.y;
		up < map.height && map.get(start.x, up) == objColor;
		++up)

		++objHeight;

	if (objHeight > KeyHeight) return Door;
	topLeft.y += objHeight;

	for (int down = start.y;
		down >= 0 && map.get(start.x, down) == objColor;
		--down)

		++objHeight;

	if (objHeight != KeyHeight) return Door;
	topLeft.y -= objHeight;

	//Check Width
	size_t objWidth = 0;

	for (int right = start.x;
		right < map.width && map.get(right, start.y) == objColor;
		++right)

		++objWidth;

	if (objWidth > KeyWidth) return Door;
	topLeft.x += objWidth;

	for (int left = start.x;
		left >= 0 && map.get(left, start.y) == objColor;
		--left)

		++objWidth;

	if (objWidth != KeyWidth) return Door;
	topLeft.x -= objWidth;

	//Check Fullnes
	for (size_t i = 0; i < KeyWidth; ++i)
		for (size_t j = 0; j < KeyHeight; ++j)
			if (map.get(i, j) != objColor) return Door;

	return Key;
}

pt findStart(const MapContainer& map) {
	pt start = { 0, 0 };

	for (size_t i = 0; i < map.size() && map[i] != EntrenceColor; ++i) {
		++start.x;
		if (start.x >= map.width) {
			start.x = 0;
			++start.y;
		}
	}

	return start;
}


void BFS_Visit_SameColor(MapContainer& map, pathToPt& curr, char& surounded, std::queue<pathToPt>& toBeScanned) {
	
	//neighboors are located  by adding the curr pt
	//	to one of the offsets
	Side sides[4] = { left, up, right, down };

	for (size_t i = 0; i < 4; ++i) {
		pt neighbour = curr.first;
		neighbour.x += offsets[i].x;
		neighbour.y += offsets[i].y;

		if (neighbour.x > map.width || neighbour.y > map.height || neighbour.x == map.width ||neighbour.y == map.height) continue;

		Pixel color = map.get(neighbour);
		// check if it's part of the obj
		if ((color & 0x00FFFFFF) == map.get(curr.first)) { 
			surounded |= sides[i];
			// check if it's visited
			if (color & 0xFF) {
				//mark as visited
				map.set(neighbour, color & 0x00FFFFFF);
				toBeScanned.push(Pair<pt, Path>(neighbour, curr.second));
			}
		}
	}
}


// This is basically BFS with the idea of
//	getting the borders of an object
std::queue<pathToPt> getOuterBorders(MapContainer& map, const pathToPt& start) {
	std::queue<pathToPt> borders;

	const Pixel& objColor = map.get(start.first.x, start.first.y);
	map.set(start.first, objColor & 0x00FFFFFF);

	std::queue<pathToPt> toBeScanned;
	toBeScanned.push(start);

	// the last 4 bits are going to represent the sides where
	//	there is another pixel from the object
	//	The scan is going to happen clockwise
	char surounded = 0; 
	
	while (!toBeScanned.empty()) {
		pathToPt& curr = toBeScanned.front();
		Path PathToNeighbour(curr.second);
		PathToNeighbour.addPoint(curr.first);

		BFS_Visit_SameColor(map, curr, surounded, toBeScanned);

		pt p = curr.first;
		if (surounded & left) {
			p.x += offsets[0].x;
			p.y += offsets[0].y;
			borders.push(Pair<pt, Path>(p, PathToNeighbour));
		}
		if (surounded & up) {
			p.x += offsets[1].x;
			p.y += offsets[1].y;
			borders.push(Pair<pt, Path>(p, PathToNeighbour));
		}
		if (surounded & right) {
			p.x += offsets[2].x;
			p.y += offsets[2].y;
			borders.push(Pair<pt, Path>(p, PathToNeighbour));
		}
		if (surounded & down) {
			p.x += offsets[3].x;
			p.y += offsets[3].y;
			borders.push(Pair<pt, Path>(p, PathToNeighbour));
		}
		toBeScanned.pop();
	}

	return borders;
}


// This is a bad way to do it but I don't have time to make a
//	new container
void appendQueue(std::queue<pathToPt>& what, std::queue<pathToPt>& to) {
	while (!what.empty()) {
		to.push(what.front());
		what.pop();
	}
}

bool contains(const std::vector<Pixel>& keys, Pixel p) {
	for (size_t i = 0; i < keys.size(); ++i)
		if (keys[i] == p) return true;
	return false;
}

void removeQueue(std::vector<std::queue<pathToPt>>& arr, size_t i) {
	std::swap(arr[i], arr.back());
	arr.pop_back();
}


//
// We're gonna be using the Alpha for diferntiating
//	visited and unvisited (hmm) pixels
std::vector<Path> solve(MapContainer& map) {
	pt start = findStart(map);

	pathToPt init = Pair<pt, Path>(start, Path()); // This conversion breaks it
	std::queue<pathToPt> toBeScanned = getOuterBorders(map, init);

	std::vector<std::queue<pathToPt>> notAccessible;
	// This is where the logic starts 
	//	to not be very good (assuming that it was in the begining)
	//	

	std::vector<Pixel> keys;

	std::vector<Path> solution;

	while (!toBeScanned.empty()) {
		pathToPt& p = toBeScanned.front();

		Pixel color = map.get(p.first);
		if (color == CoridorColor) {
			std::queue<pathToPt> temp = getOuterBorders(map, p);
			appendQueue(temp, toBeScanned);
		}
		else if (color == ExitColor) {
			solution.push_back(p.second);
			return solution;
		}
		else if (color != WallColor) {
			ObjType t = identifyObj(map, p.first);

			if (t == Key) {
				std::queue<pathToPt> temp = getOuterBorders(map, p);
				appendQueue(temp, toBeScanned);
				if (!contains(keys, color)) {
					keys.push_back(map.get(p.first));
					for (size_t i = 0; i < notAccessible.size(); ++i) {
						pathToPt& currrDoor = notAccessible[i].front();
						if (color == map.get(currrDoor.first)) {
							appendQueue(notAccessible[i], toBeScanned);
							removeQueue(notAccessible, i);
						}
					}
				}
			}
			else if (t == Door) {
				std::queue<pathToPt> temp = getOuterBorders(map, p);
				if (contains(keys, map.get(p.first)))
					appendQueue(temp, toBeScanned);
				else
					notAccessible.push_back(temp);
			}
		}//this should be all cases

		toBeScanned.pop();
	}

	return std::vector<Path>();
}
//if this works 


#endif






