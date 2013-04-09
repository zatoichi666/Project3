#ifndef FILEFIND_H
#define FILEFIND_H

#include <string>
#include <vector>
#include <iostream>

#include "FileSystem.h"

using namespace FileSystem;
std::vector<std::string> findFilesWithExtensionsMatching(std::vector<std::string> fileExtensionsToFind, Directory dir);
void findFile_r(std::vector<std::string> &refvecFiles,
			   Directory &dir, bool isRecursive, std::vector<std::string> fileExtList);

const std::vector<std::string> getFileExtensionList(void);
void printTestFolderContents();

#endif