/////////////////////////////////////////////////////////////////////////////
// Packetizer.cpp - Streams in files and prepares them in a object for     //
//                  Socket communication for project 3, CSE-687            //
//                  Spring 2013                                            //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Packetizer.h"
#include "filefind.h"
#include "FileSystem.h"
#include "base64.h"
#include "md5.h"

using namespace FileSystem;

std::string Packetizer::getFileName()
{
	return fileName;
}

size_t Packetizer::getUncompressedSizeInBytes()
{
	return uncompressedSizeInBytes;
}

Packetizer::Packetizer(std::string Path)
{
	fileName = Path.substr(Path.find_last_of("\\")+1);
	std::cout << " Packetizer: Preparing file " << fileName << " for socket communication";
	File inFile(Path);
	FileInfo inFile_fi(Path);

	size_t fileSize = inFile_fi.size();
	uncompressedSizeInBytes = fileSize;
	//std::cout << "File size in bytes: " << uncompressedSizeInBytes << "\n";

	size_t packetCount = fileSize / CHUNK_SIZE;
	if (fileSize % CHUNK_SIZE > 0)
		packetCount++;

	inFile.open(File::in, File::binary);
	
	if(!inFile.isGood())
	{
		std::cout << "\n  can't open binary file\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << Path::getFullFileSpec(inFile.name()) << "\n";
	}
	else
	{
		Block b;
		std::string fileContents;

		while(inFile.isGood())
		{			
			b = inFile.getBlock(CHUNK_SIZE);
			std::string packet;
			for (size_t i=0;i<b.size();i++)
				packet.push_back(b[i]);
			fileContents += packet;
			Packetizer::PacketList.push_back(base64::base64_encode(reinterpret_cast<const unsigned char*>(packet.c_str()), packet.length()));
		}
		std::cout << " MD5: " << md5(fileContents.c_str()) << "\n";
	}
}

std::string Packetizer::operator[](size_t i)
{
	return PacketList[i];
}

void Packetizer::toFile(std::string Path)
{
	std::cout << "DePacketizing: " << Path << "\n";
	File outFile(Path);
	outFile.open(File::out, File::binary);
	if(outFile.isGood())
	{
		for (size_t i=0;i<PacketList.size(); i++)
		{
			std::string s = base64::base64_decode(PacketList[i]);
			Block b(&s[0],&s[s.size()]);
			outFile.putBlock(b);

		}
		std::cout << "\n";
	}
}


size_t Packetizer::size()
{
	return PacketList.size();
}

#ifdef TEST_PACKETIZER

int main()
{
	std::vector<std::string> vecTxtFilesNonR;
	Directory dir;

	std::string path = Directory::getCurrentDirectory() + "\\test";
	dir.setCurrentDirectory(path);

	std::cout << "\nStarting in "<< path << " and running nonrecursively." << std::endl;

	findFile_r(vecTxtFilesNonR, dir, false, getFileExtensionList() );

	std::cout << "Found "<< vecTxtFilesNonR.size() << " files\n";
	for (size_t i=0;i<vecTxtFilesNonR.size();i++)
	{
		Packetizer p(vecTxtFilesNonR[i]);
		std::cout << "In file: " << p.getFileName() << "\n";
		std::cout << "Packetized "<< p.size() << " packets.\n";
		p.toFile("depacketized.bin");
	}
}




#endif