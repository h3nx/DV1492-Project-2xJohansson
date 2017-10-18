#include "filesystem.h"
#include <sstream>

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
	
	newFolder.fileSize = 0;
	newFolder.link = 0;
	newFolder.accessRights = 0;
	
	std::string parent_str = mMemblockDevice.readBlock(newFolder.parent).toString();
	Entry parent;
	this->readBlock(parent_str, &parent);
	parent.data = newFolder.blockId;
	this->mMemblockDevice.writeBlock(newFolder.parent, this->toString(parent));
	//
	parent_str = mMemblockDevice.readBlock(newFolder.parent).toString();
	//
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
	//int blockId = this->findBlock(path);
	int blockId = 0;

	std::string block = this->mMemblockDevice.readBlock(blockId).toString();

	Entry entry;
	this->readBlock(block, &entry);
	
	std::vector<int> ids; 
	std::vector<std::string> names;
	std::vector<bool> folder;
	findContentFolder(entry.data, ids, names, folder);

	std::string fr = "folder", fe = "file";
	for (int i = 0; i < ids.size(); i++) {		
		result << names[i] << ", ";
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



	if (mMemblockDevice.writeBlock(0, this->toString(root)) == 1)
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

}

void FileSystem::findContentFolder(std::string data, std::vector<int> &ids, std::vector<std::string> &names, std::vector<bool> &folder)
{
	std::string tmpBlock;
	int i = 0;
	std::string id("null");
	while (i < data.length() - 3) {
		id = data.substr(i, 3);
		if (id[0] == '\0')
			break;
		ids.push_back(std::stoi(id));
		tmpBlock = this->mMemblockDevice.readBlock(std::stoi(id)).toString();
		names.push_back(tmpBlock.substr(0, NAME_SIZE));
		folder.push_back(std::stoi(tmpBlock.substr(NAME_SIZE)));
		i += 3;
	}
}


std::string FileSystem::load(std::string filePath)
{
	std::string toRet = "incorrect path";





	return toRet;
}
std::string FileSystem::save(std::string filePath)
{
	std::string toRet = "Could not save to: " + filePath;
	std::ofstream file(filePath);
	
	unsigned int blockToRead = 0;
	if (file.is_open())
		toRet = "saved to: " + filePath;
	while (file.is_open() && blockToRead < 250)
	{
		if (this->block_map[blockToRead])
		{
			file << this->mMemblockDevice.readBlock(blockToRead).toString() << " wtf\n\n";
		}
		else
		{
			file << "0\n\n";
		}
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




