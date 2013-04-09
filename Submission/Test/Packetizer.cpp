#include <iostream>

#include "Packetizer.h"
#include "filefind.h"
#include "FileSystem.h"
#include "base64.h"

using namespace FileSystem;

std::string Packetizer::getFileName()
{
	return fileName;
}

Packetizer::Packetizer(std::string Path)
{

	std::cout << "Packetizing: " << Path << "\n";

	fileName = Path.substr(Path.find_last_of("\\")+1);

	File inFile(Path);
	FileInfo inFile_fi(Path);

	size_t fileSize = inFile_fi.size();
	size_t packetCount = fileSize / CHUNK_SIZE;
	if (fileSize % CHUNK_SIZE > 0)
		packetCount++;

	inFile.open(File::in, File::binary);
	std::cout << "\n  copying " << inFile.name().c_str() << " to c:/temp";
	if(!inFile.isGood())
	{
		std::cout << "\n  can't open binary file\n";
		std::cout << "\n  looking for:\n  ";
		std::cout << Path::getFullFileSpec(inFile.name()) << "\n";
	}
	else
	{
		Block b;
		while(inFile.isGood())
		{			
			b = inFile.getBlock(CHUNK_SIZE);
			std::string packet;

			for (int i=0;i<b.size();i++)
				packet.push_back(b[i]);

			Packetizer::PacketList.push_back(base64::base64_encode(reinterpret_cast<const unsigned char*>(packet.c_str()), packet.length()));
		}
		std::cout << "Done packetizing.  Made " << PacketList.size() << " packets\n";
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
		p.toFile("C:\\Wondervision2.m4a");
	}
}




#endif