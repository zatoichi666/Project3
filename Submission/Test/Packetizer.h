#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

//#include "base64.h"


#define CHUNK_SIZE 1024

class Packetizer
{
private:
	std::vector<std::string> PacketList;
	std::string fileName;
public:
	std::string getFileName();
	std::string operator[](size_t index);
	Packetizer(std::string Path);
	void toFile(std::string Path);
	size_t size();
};





#endif