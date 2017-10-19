#include "filesystem.h"
#include <sstream>
#include <algorithm>
FileSystem::FileSystem() {
	for (int i = 0; i < 250; i++) block_map[i] = 0;

	this->initRoot();
}

FileSystem::~FileSystem() {

}

std::string FileSystem::createFolder(std::string path)
{
	Entry newFolder;
	
	std::string folderName, parentPath;
	int c = path.length();
	while (c >= 0) {
		if (path[c] != '/') {
			folderName += path[c];
			c--;
		}		
		else {
			folderName = path.substr(c + 1);
			parentPath = path.substr(0, c);
			break;
		}
	}
	newFolder.name = folderName;
	newFolder.parent = this->findBlock(parentPath);
	

	int freeBlock = 0;
	while (block_map[freeBlock]) {
		freeBlock++;
		if (freeBlock >= 250)
			return "error: no space";
	}
	newFolder.blockId = freeBlock;
	newFolder.folder = 1;
	newFolder.fileSize = 0;
	newFolder.link = 0;
	newFolder.accessRights = 0;
	
	std::string parent_str = mMemblockDevice.readBlock(newFolder.parent).toString();
	Entry parent;
	this->readBlock(parent_str, &parent);
	parent.data.append(this->IdToStr(newFolder.blockId, 3));
	this->mMemblockDevice.writeBlock(newFolder.parent, this->toString(parent));
	//testing
	//parent_str = mMemblockDevice.readBlock(newFolder.parent).toString();
	//this->readBlock(parent_str, &parent);

	if (mMemblockDevice.writeBlock(freeBlock, this->toString(newFolder)) == 1) {
		this->block_map[freeBlock] = 1;
		return std::string("folder created");
	}
	else
		return std::string("error: failed writing disk");
}

std::string FileSystem::listDir(std::string path)
{
	std::stringstream result;
	result << "Name:\t\t\t\tType:\n";
	//int blockId = this->findBlock(path);
	int blockId = 0;

	std::string block = this->mMemblockDevice.readBlock(blockId).toString();

	Entry entry;
	this->readBlock(block, &entry);
	
	std::vector<int> ids; 
	std::vector<std::string> names;
	std::vector<bool> folder;
	findContentFolder(entry.data, ids, names, folder);

	std::string fr = "(folder)", fe = "(file)";
	for (int i = 0; i < ids.size(); i++) {		
		result << names[i] << "\t";
				
		int nrOfTabs = 3 - (names[i].length()/ 8) % 8;
		for (int j = 0; j < nrOfTabs; j++)
			result << "\t";		

		if(folder[i])
			result << fr;
		else
			result << fe;
		result << std::endl;
	}

	return result.str();
}

void FileSystem::initRoot()
{
	Entry root;
	root.name = ".\x3";
	root.blockId = 0;
	root.folder = 1;
	root.parent = 0;
	root.fileSize = 0;
	root.link = 0;
	root.accessRights = 0;
	root.data = "";


	std::string ddd = this->toString(root);
	if (mMemblockDevice.writeBlock(0, ddd) == 1)
		this->block_map[0] = 1;
}
int FileSystem::findBlock(std::string location)
{
	return 0;
}

std::string FileSystem::toString(Entry item){
	std::string result;
	//int i = 0;
		
	//name			64
	result += item.name;
	for (int i = item.name.size(); i < NAME_SIZE; i++)
		result += '\0';
		 
	//block id		3
	result += IdToStr(item.blockId, 3);

	//folder		1
	result += std::to_string(item.folder);

	//parent		3
	result += IdToStr(item.parent, 3);

	//fileSize		8
	result += IdToStr(item.fileSize, 8);

	//link			3
	result += IdToStr(item.fileSize, 3);

	//accessrights	1
	result += std::to_string(item.accessRights);

	//data
	result += item.data;	
	int remaining = 512 - result.length();
	for (int p = 0; p < remaining; p++)
		result += '\0';

	return result;
}
std::string FileSystem::toString(DataBlock item) {
	return "0";
}

void FileSystem::readBlock(std::string block, Entry *entry)
{
	char name[NAME_SIZE];

	//name
	int i = 0;
	entry->name = block.substr(0, NAME_SIZE);
	entry->name.erase(std::remove(entry->name.begin(), entry->name.end(), '\0'), entry->name.end());
	i = NAME_SIZE;

	//blockiD
	std::string id;
	entry->blockId = std::stoi(block.substr(i, 3));
	i += 3;

	//folder
	id = block[i];
	entry->folder = std::stoi(id);
	i++;

	//parent
	entry->parent = std::stoi(block.substr(i, 3));
	i += 3;

	//fileSize	
	entry->fileSize = std::stoi(block.substr(i, 8));
	i += 8;

	//link
	entry->link = std::stoi(block.substr(i, 3));
	i += 3;

	//accessRights
	id = block[i];
	entry->accessRights = std::stoi(id);
	i++;

	//data
	entry->data = block.substr(i);
	entry->data.erase(std::remove(entry->data.begin(), entry->data.end(), '\0'), entry->data.end());
}

void FileSystem::findContentFolder(std::string data, std::vector<int> &ids, std::vector<std::string> &names, std::vector<bool> &folder)
{
	std::string tmpBlock;
	Entry tmpEntry;
	int i = 0;
	std::string id("null");
	while (i <= data.length()) {
		id = data.substr(i, 3);
		if (id[0] == '\0')
			break;
		ids.push_back(std::stoi(id));
		tmpBlock = this->mMemblockDevice.readBlock(std::stoi(id)).toString();
		this->readBlock(tmpBlock, &tmpEntry);
		names.push_back(tmpEntry.name);
		folder.push_back(tmpEntry.folder);
		i += 3;
	}
}


std::string FileSystem::load(std::string filePath)
{
	std::string toRet = "";
	std::ifstream file(filePath);
	unsigned int blockToWrite = 0;
	std::string line = "";
	std::string lastLine = "";
	std::string toWrite = "";
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (line == "\x3" && toWrite != "")
			{
				if (this->mMemblockDevice.writeBlock(blockToWrite, toWrite) != 1)
					toRet += "block at " + std::to_string(blockToWrite) + " corrupted and were not loaded\n";
				else
					block_map[blockToWrite] = 1;
				blockToWrite++;
				toWrite = "";
			}
			else if (line == "0" && lastLine == "\x3")
			{
				blockToWrite++;
				toWrite = "";
			}
			else
			{
				if(toWrite != "")
					toWrite += line;
			}
				
			lastLine = line;
		}
	}
	else
		return "incorrect path";

	if (toRet == "")
		return "image Loaded";
	return toRet;
}
std::string FileSystem::save(std::string filePath)
{
	std::string toRet = "Could not save";
	std::ofstream file(filePath);
	unsigned int blockToRead = 0;

	if (file.is_open())
		toRet = "saved";
	while (file.is_open() && blockToRead < 250)
	{
		if (this->block_map[blockToRead])
		{
			toRet = this->mMemblockDevice.readBlock(blockToRead).toString();
			file << toRet << "\n";
		}
		else
		{	
			file << "0\n";
		}
		file << "\x3\n";
		blockToRead++;
	}
	file.close();
	return toRet;
}


std::string FileSystem::IdToStr(unsigned int id, unsigned int max_size)
{
	std::string num = std::to_string(id);

	while (num.length() < max_size)
		num = "0" + num;

	return num;
}




