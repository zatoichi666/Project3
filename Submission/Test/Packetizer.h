/////////////////////////////////////////////////////////////////////////////
// Packetizer.h   - Streams in files and prepares them in a object for     //
//                  Socket communication for project 3, CSE-687            //
//                  Spring 2013                                            //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

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
	size_t uncompressedSizeInBytes;
public:
	std::string getFileName();
	std::string operator[](size_t index);
	Packetizer(std::string Path);
	void toFile(std::string Path);
	size_t size();
	size_t getUncompressedSizeInBytes();
};





#endif