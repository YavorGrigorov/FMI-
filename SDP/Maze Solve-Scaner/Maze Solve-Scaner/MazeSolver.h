

#ifndef MAZE_SOLVER_HEADER_INCLUDED
#define MAZE_SOLVER_HEADER_INCLUDED


#include "Point.h"
#include "SourceArray.h"

#include <vector>

namespace np {
	typedef std::vector<np::Point> Path;

	//
	// For identifying objects
	enum ObjType {
		Enterence, Key, Door, Exit, Wall, Coridor
	};

	typedef std::vector<std::pair<np::Point, ObjType>> ObjContainer;

	//
	/// Not great, not terrible
	//
	// TODO:: Add description
	Path solve2(np::SourceArray& map);

	//
	// Scans the maze and fills the source array
	//
	///	@param map - the source array that is filled. If points in it are not 
	///		marked as unvisited they will not be scanned
	///	@param start - the point that is considered a starting point for the graph
	///	@param includeDoors - when it's false, the function will consider doors as
	///		walls and will not scan them. when it's true, it will scan them and go
	///		through them
	//
	ObjContainer fillSourceArrayFromPt(SourceArray& map, const Point& start, const std::vector<color_t>& unpassable, bool includeDoors = true);

	//
	// Finds the enterence to the maze and then it fills the source array with
	//	the first point of the enterence as starting point
	//
	///	@param map - the source array that is filled. If points in it are not 
	///		marked as unvisited they will not be scanned
	///	@param includeDoors - when it's false, the function will consider doors as
	///		walls and will not scan them. when it's true, it will scan them and go
	///		through them
	//
	ObjContainer fillSourceArrayFromStart(SourceArray& map, const std::vector<color_t>& unpassable, bool includeDoors = true);

	//
	//	Finds the shortest path from pt from to pt to using A* algorithm
	//	 with heuristic the dist from the current point to the to point. //needs to be written better
	//
	int setPathFromTo(const Point&from, const Point& to, SourceArray& map, const std::vector<color_t>& unpassable);
	
}


#endif



