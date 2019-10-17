



#include "Logger.h"

#include "SourceArray.h"
#include "ImageLoader.h"
#include "MazeSolver.h"

//See how debug was enabled
#undef LOG

int main(int argc, char* argv[]) {

	LOG_TEXT("Starting Program "); LOG_TEXT(time(NULL)); LOG_ENDL;

	np::SourceArray map;
	Status s = loadBmp(argv[1], map);
	LOG_TEXT("	File read status ");
	LOG_TEXT(s);
	LOG_ENDL;
	if (s != ALL_OK) return s;

	np::Path p = np::solve2(map);

	for (const auto& i : p)
		map.setPtColor(i.x, i.y, PATH_COLOR);

	LOG_TEXT("	Saving img\n");
	saveBmp(argv[2], map, map.getHeight(), map.getWidth());
	
	return 0;
}

