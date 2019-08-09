

#ifndef MAZE_SOLVER_INCLUDED
#define MAZE_SOLVER_INCLUDED

#include "MazeRules.h"
#include "Point.h"
#include "SourceArray.h"

#include <vector>
#include <queue>

namespace np {
///////////////////// To be removed from header /////////////////////////////////////////	
			//it;s used somewhere
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
			
			enum Side {
				left = 8,
				up = 4,
				right = 2,
				down = 1
			};
			//doesn't check the shape of the obj!!!
			ObjType identifyObj(const SourceArray& map, const Point& start) ;
			
			void BFS_Visit_SameColor(MapContainer& map, pathToPt& curr, char& surounded, std::queue<pathToPt>& toBeScanned);
			
			// This is basically BFS with the idea of
			//	getting the borders of an object
			std::queue<pathToPt> getOuterBorders(MapContainer& map, const pathToPt& start);
			
			// This is a bad way to do it but I don't have time to make a
			//	new container
			void appendQueue(std::queue<pathToPt>& what, std::queue<pathToPt>& to);
			
			bool contains(const std::vector<Pixel>& keys, Pixel p) ;
			
			void removeQueue(std::vector<std::queue<pathToPt>>& arr, size_t i);
///////////////////////////////////////////////////////////////////////////////////////////
	
	//
	// We're gonna be using the Alpha for diferntiating
	//	visited and unvisited (hmm) pixels
	std::vector<Path> solve(MapContainer& map) ;
}

#endif






