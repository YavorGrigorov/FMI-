

#ifndef MAZE_SOLVER_HEADER_INCLUDED
#define MAZE_SOLVER_HEADER_INCLUDED


#include "Point.h"
#include "SourceArray.h"

#include <queue>
#include <vector>

//
// This is for testing..

namespace np {
	//
	// For identifying objects
	enum ObjType {
		Enterence, Key, Door, Exit, Wall, Coridor
	};

	typedef std::vector<std::pair<np::Point, ObjType>> ObjContainer;

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
	ObjContainer fillSourceArrayFromPt(SourceArray& map, const Point& start, bool includeDoors = true);

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
	ObjContainer fillSourceArrayFromStart(SourceArray& map, bool includeDoors = true);

}


#endif



