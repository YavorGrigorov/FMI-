


#ifndef IMAGE_LOADER_HEADER_INCLUDED__
#define	IMAGE_LOADER_HEADER_INCLUDED__

#include <fstream>
#include <iostream>
#include "SourceArray.h"

using std::clog;

enum Status {
	FAIL,
	FAILED_TO_READ,
	FAILED_TO_OPEN,
	FAILED_TO_WRITE,
	WRONG_FILE_TYPE,
	ALL_OK
};

typedef np::SourceArray Container;
typedef Container::Iterator iterator;
typedef Container::ConstIterator citerator;

//TODO::
#define PIXEL_ARRAY_OFFSET_POS 10
#define IMAGE_MEASURMENTS_POS 18

//Basically, the rules for the picture
//They might change, based on how much of a difference
//	there is between the different types of bmp.
Status isValid(std::ifstream& in) {
	unsigned buff = 0;
	
	//Has BM signature
	in.read((char*)&buff, 2); 
	if (buff != 0x00004D42) return WRONG_FILE_TYPE;

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

size_t calcPadding(size_t width) {
	size_t pad = 4 - (width * 3) % 4;
	return pad == 4 ? 0 : pad;
}

Status loadBmp(const char* filePath, Container& container) {
	clog << "Loading file " << filePath << std::endl;
	if (!filePath) return FAIL;

	std::ifstream file(filePath, std::ios::binary | std::ios::beg);
	if (!file) return FAILED_TO_OPEN;

	clog << "Checking validity of bmp file\n";
	//probably, I should rename these
	Status failStatus = isValid(file);
	if (failStatus != ALL_OK) return failStatus;

	clog << "Getting pixel array start\n";
	//Getting the pixel arr begining
	size_t imgArrStart = 0;
	file.seekg(10, std::ios::beg);
	file.read((char*)&imgArrStart, 4);

	clog << "Getting image measurements\n";
	//Measurments
	file.seekg(18, std::ios::beg);
	size_t width = 0, height = 0;
	file.read((char*)&width, 4);
	file.read((char*)&height, 4);
	
	container.setHeight(height);
	container.setWidth(width);
	
	size_t size = width * height;
	
	iterator iter = container.beg();
	iterator endIter = container.end();
	
	unsigned paddingSize = calcPadding(width);
	clog << "Calculating Padding : " << paddingSize << std::endl;
	size_t i = 0;

	clog << "Starting to read pexel array from pos " << imgArrStart << std::endl;
	file.seekg(imgArrStart);

	np::color_t pixel = 0xFF000000;
	while (file && iter != endIter) {
		clog << "Setting pt " << iter << " to ";
		
		file.read((char*)&pixel, 3);
		iter->color = pixel;

		clog << iter << std::endl;

		++iter;
		++i;

		if (i == width) {
			clog << "Skipping padding " << paddingSize << " bytes\n";
			file.seekg(paddingSize, std::ios::cur);
			i = 0;
		}
	}

	if (iter != endIter && !file.eof()) return FAILED_TO_READ;

	clog << "Closing file\n";
	file.clear();
	file.close();

	return ALL_OK;
} //seems legit


unsigned createHeader(std::ofstream& file, size_t width, size_t height) {

	size_t buff = 0;

	// BM Signature
	file.put(0x42);
	file.put(0x4d);
	
	// Size
	// Each row has to end on a dword (4 bytes)
	unsigned padding = calcPadding(width);
	//54 is the size of both headers
	buff = 54 + (width * 3 + padding)* height; 
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

	// Bits per pixel
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

	// Used colors
	// 0 is used to specify that we don't have a restriction (?)
	buff = 0;
	file.write((char*)&buff, 4);
	// Color pallete and important colors
	//	All colors are important <3
	buff = 0;
	file.write((char*)&buff, 4);

	return padding;
}


Status saveBmp(const char* filePath, const Container& container, size_t height, size_t width) {
	clog << "Saving to file " << filePath << std::endl;
	if (!filePath) return FAIL;

	std::ofstream file(filePath, std::ios::binary);
	if (!file) return FAILED_TO_OPEN;
	
	unsigned paddingSize = createHeader(file, width, height);
	unsigned padding = 0;
	if (!file) return FAILED_TO_WRITE;
	
	file.seekp(54);
	
	size_t i = 0;
	size_t pos = 54;
	citerator endIter = container.cend();
	
	clog << "Map width " << container.getWidth()
		<< "\n Map height " << container.getHeight();

	for ( 	citerator iter = container.cbeg();
			file && iter != endIter;
			++iter) {

		clog << " Writing pixel from arr " << iter << " ::\n"
			<< " Byte pos " << pos << " val " << std::hex << (iter->color)
			<< std::dec
			<< "   Pos in file " << file.tellp()
			<< std::endl;
		file.write((char*)&(iter->color), 3);
		++i;
		pos += 3;

		if (i == width) {
			clog << "Putting padding " << paddingSize << " Null bytes\n";
			file.write((char*)&padding, paddingSize);
			i = 0;
			pos += paddingSize;
		}
	}

	clog << "Closing File\n";

	if (!file) return FAILED_TO_WRITE;

	file.clear();
	file.close();
	return ALL_OK;
}


#endif



