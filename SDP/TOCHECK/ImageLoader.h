


#ifndef IMAGE_LOADER_HEADER_INCLUDED__
#define	IMAGE_LOADER_HEADER_INCLUDED__

#include <fstream>
#include "Objects.h"

enum Status {
	FAIL,
	FAILED_TO_READ,
	FAILED_TO_OPEN,
	FAILED_TO_WRITE,
	WRONG_FILE_TYPE,
	ALL_OK
};

//Basically, the rules for the picture
//They might change, based on how much of a difference
//	there is between the different types of bmp.
Status isValid(std::ifstream& in) {
	unsigned buff = 0;

	//Has BM signature
	in.read((char*)&buff, 2);
	if (buff != 0x4D42) return WRONG_FILE_TYPE;

	//Is one plane
	in.seekg(26, std::ios::beg);
	in.read((char*)&buff, 2);
	if (buff != 1) return WRONG_FILE_TYPE;

	//Is 24-bit bitmap
	in.read((char*)&buff, 2);
	if (buff != 24) return WRONG_FILE_TYPE;

	//Is not compressed
	in.read((char*)&buff, 2);
	if (buff != 0) return WRONG_FILE_TYPE;

	return ALL_OK;
}


typedef MapContainer Container;

Status loadBmp(const char* filePath, Container& buffer, size_t& width, size_t& height) {
	if (!filePath) return FAIL;

	std::ifstream file(filePath, std::ios::binary | std::ios::beg);
	if (!file) return FAILED_TO_OPEN;

	//probably, I should rename these
	Status failStatus = isValid(file);
	if (failStatus != ALL_OK) return failStatus;

	//Getting the pixel arr begining
	size_t imgArrStart = 0;
	file.seekg(10, std::ios::beg);
	file.read((char*)&imgArrStart, 4);

	//Measurments
	file.seekg(18, std::ios::beg);
	file.read((char*)&width, 4);
	file.read((char*)&height, 4);

	file.seekg(imgArrStart, std::ios::beg);
	size_t size = width * height;

	//char *filearr = new char[size];
	//auto f = file.tellg();
	//file.read(filearr, size);
	//auto a = file.tellg() - f;
	//file.seekg(f);

	unsigned blue = 0, red = 0, green = 0;
	int i = 0;
	while (file && size > 0) {

		++i;

		file.read((char*)&blue, 1);
		file.read((char*)&green, 1);
		file.read((char*)&red, 1);

		//Pixel p;
		//p.p[0] = red;
		//p.p[1] = green;
		//p.p[2] = blue;
		//p.p[3] = 0xff;
		//buffer.push_back(p);
		buffer.push_back((0xFF << 24) | (blue << 16) | (green << 8) | red);
		--size;
	}

	if (size != 0 && !file.eof()) return FAILED_TO_READ;

	file.clear();
	file.close();

	return ALL_OK;
} //seems legit


void createHeader(std::ofstream& file, size_t width, size_t height) {

	size_t buff = 0;

	// BM Signature
	file.put(0x42);
	file.put(0x4d);

	// Size
	buff = 54 + width * height * 3; //54 is all the bytes before the pixel arr
	file.write((char*)&buff, 4);

	// Rezereved space for sth (I couldn't find what for)
	buff = 0;
	file.write((char*)&buff, 4);

	// Image data offset
	buff = 54;
	file.write((char*)& buff, 4);

	// Size of the header
	buff = 40;
	file.write((char*)& buff, 4);

	// Width and Height
	file.write((char*)&width, 4);
	file.write((char*)&height, 4);

	// Planes
	buff = 1;
	file.write((char*)&buff, 2);

	// Bits pre pixel
	buff = 24;
	file.write((char*)&buff, 2);

	// Compression method and size 
	buff = 0;
	file.write((char*)&buff, 4); // Because there is no compression
	file.write((char*)&buff, 4); //		the size can be left as 0

								 // Verical and horisontal resolution
								 //	I have no idea how to calculate these
	buff = 60;
	file.write((char*)&buff, 4);
	file.write((char*)&buff, 4);
	// Hopefully you wouldn't try and print these :D

	// Color pallete and important colors
	//	All colors are important
	buff = 0;
	file.write((char*)&buff, 4);
	// I'm not that sure
	file.write((char*)&buff, 4);

}


Status saveBmp(const char* filePath, const Container& container, size_t height, size_t width) {
	if (!filePath) return FAIL;

	std::ofstream file(filePath, std::ios::beg | std::ios::binary);
	if (!file) return FAILED_TO_OPEN;

	createHeader(file, width, height);
	if (!file) return FAILED_TO_WRITE;

	unsigned redMask = 0x000000FF;
	unsigned greenMask = 0x0000FF00;
	unsigned blueMask = 0x00FF0000;

	for (size_t i = 0; file && i < container.size(); ++i) {
		char blue = (container[i] & blueMask) >> 16;
		char green = (container[i] & greenMask) >> 8;
		char red = container[i] & redMask;

		file.put(blue);
		file.put(green);
		file.put(red);
	}

	if (!file) return FAILED_TO_WRITE;

	return ALL_OK;
}


#endif



