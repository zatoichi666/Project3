/////////////////////////////////////////////////////////////////////////////
// filefind.cpp - Recursively find files by extension                      //
//                                                                         //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2012                             //
// Platform:    Dell Dimension E6510, Windows 7                            //
// Application: CSE-687                                                    //
// Author:      Matt Synborski                                             //
//              matthewsynborski@gmail.com                                 //
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <iostream>
#include "FileSystem.h"

using namespace FileSystem;
std::vector<std::string> findFilesWithExtensionsMatching(std::vector<std::string> fileExtensionsToFind, Directory dir)
{
	std::vector<std::string> file_list, intermediate_file_list;
	for(std::vector<std::string>::iterator it_x = fileExtensionsToFind.begin(); it_x != fileExtensionsToFind.end(); ++it_x) 
	{
		intermediate_file_list = Directory::getFiles(dir.getCurrentDirectory(),"*." + *it_x);

		for(std::vector<std::string>::iterator it_y = intermediate_file_list.begin(); it_y != intermediate_file_list.end(); ++it_y)
		{
			file_list.push_back(Directory::getCurrentDirectory() + "\\" + *it_y);
		}
	}
	return file_list;
}

void findFile_r(std::vector<std::string> &refvecFiles,
				Directory &dir, bool isRecursive, std::vector<std::string> fileExtList)
{

	std::string     strFilePath;
	std::string     strPattern;
	std::string     strExtension;

	std::vector<std::string> fileList;
	std::vector<std::string> dirList;

	fileList = findFilesWithExtensionsMatching( fileExtList, dir);

	for(size_t i=0; i<fileList.size(); ++i)
	{
		refvecFiles.push_back(fileList[i].c_str());
	}

	dirList = dir.getDirectories();

	if (dirList.size() > 2)
	{	
		for(size_t i=2; i<dirList.size(); ++i)
		{			
			if (i == 2)
			{
				dir.setCurrentDirectory(dirList[i]);
			}
			else
			{
				dir.setCurrentDirectory("..\\" + dirList[i]);
			}	
			if (isRecursive)
				findFile_r(refvecFiles, dir, isRecursive, fileExtList );
		}

	}
}

const std::vector<std::string> getFileExtensionList(void)
{
	std::vector<std::string> fileExtensions;
	fileExtensions.push_back("scn");

	return fileExtensions;
}

#ifdef TEST_FILEFIND



void printTestFolderContents()
{
	/*
	std::cout << "Unit Test for fileFind       " << std::endl;
	std::cout << "Displaying file system:      " << std::endl;
	std::cout << "                             " << std::endl;
	std::cout << "test                         " << std::endl;
	std::cout << "|   test1.h                  " << std::endl;
	std::cout << "|   test5.cpp                " << std::endl;
	std::cout << "|                            " << std::endl;
	std::cout << "L---test1                    " << std::endl;
	std::cout << "    |---test2                " << std::endl;
	std::cout << "    |       test2.h          " << std::endl;
	std::cout << "    |                        " << std::endl;
	std::cout << "    L---test3                " << std::endl;
	std::cout << "        L---test4            " << std::endl;
	std::cout << "            |   test7.cpp    " << std::endl;
	std::cout << "            |                " << std::endl;
	std::cout << "            L---test5        " << std::endl;
	std::cout << "                    test5.h  " << std::endl;
	std::cout << "                    test8.cpp" << std::endl << std::endl;
	*/
}

int main()
{
	//int iRC = 0;
	std::vector<std::string> vecTxtFiles;
	Directory dir;
	printTestFolderContents();
	std::string path = Directory::getCurrentDirectory() + "\\test";
	dir.setCurrentDirectory(path);
	std::cout << "Starting in "<< path << " and running recursively." << std::endl;
	findFile_r(vecTxtFiles, dir, true, getFileExtensionList() );

	for(std::vector<std::string>::iterator iterTxt = vecTxtFiles.begin();
		iterTxt != vecTxtFiles.end();
		++iterTxt)
		std::cout << *iterTxt << std::endl;

	std::vector<std::string> vecTxtFilesNonR;
	std::cout << "\nStarting in "<< path << " and running nonrecursively." << std::endl;

	findFile_r(vecTxtFilesNonR, dir, false, getFileExtensionList() );

	for(std::vector<std::string>::iterator iterTxt = vecTxtFilesNonR.begin();
		iterTxt != vecTxtFilesNonR.end();
		++iterTxt)
		std::cout << *iterTxt << std::endl;

	std::cout << "Finished" << std::endl;

	return 0;
}

#endif