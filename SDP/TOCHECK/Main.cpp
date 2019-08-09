


#include <iostream>

#include "Objects.h"
#include "ImageLoader.h"
#include "MazeSolver.h"


int main() {
	size_t size = 512;
	const char* filePath = "test.bmp";// new char[size]; //TODO';;
	
	//std::cin.getline(filePath, size);

	MapContainer map;
	if(loadBmp(filePath, map, map.width, map.height) != ALL_OK) return 1;

	std::vector<Path> solution = solve(map);
	if (!solution.empty()) {
		for (size_t i = 0; i < solution.size(); ++i)
			map.addPath(solution[i], 0xFFFF00FF);
	
	}

		saveBmp("res.bmp", map, map.height, map.width);

	//delete[] filePath;
	return 0;
}

// I'm buying drinks if this works


