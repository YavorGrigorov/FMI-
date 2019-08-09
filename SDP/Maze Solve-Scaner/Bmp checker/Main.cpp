

#include <fstream>
#include <iostream>

size_t getSize(std::ifstream& file) {
	file.seekg(std::ios::end);
	size_t size = file.tellg();
	file.seekg(std::ios::beg);
	return size;
}

int main(int argc, char* argv[]) {

	std::cout << "Starting \n";

	std::ifstream orig(argv[1], std::ios::binary);
	std::ifstream newF(argv[2], std::ios::binary);
	std::ofstream log("log.txt");
	if (!orig || !newF || !log) return 1;

	size_t origSize = getSize(orig);
	size_t newFSize = getSize(newF);

	orig.seekg(10);
	size_t width = 0;
	orig.read((char*)&width, 4);
	orig.seekg(std::ios::beg);

	log << "Original Size = " << origSize
		<< "New File Size = " << newFSize << "\n";

	size_t minSize = (origSize < newFSize ? origSize : newFSize);
	
	struct { int data : 8; } buff1, buff2;
	buff1.data = 0;
	buff2.data = 0;
	for (size_t i = 0; i < minSize; ++i) {
		orig.read((char*)&buff1, 1);
		newF.read((char*)&buff2, 1);

		if (buff1.data != buff2.data) {
			log << "   Line " << (i % width);
			log << "  Byte " << i
				<< " " << std::hex
				<< buff1.data << " != " << buff2.data 
				<< std::dec << std::endl;
		}
	}

	orig.clear();
	newF.clear();
	log.clear();
	
	orig.close();
	newF.close();
	log. close();
	
	std::cout << "Finished\n";

	return 0;
}








