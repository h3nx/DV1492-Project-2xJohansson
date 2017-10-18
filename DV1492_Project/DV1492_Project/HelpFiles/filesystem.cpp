#include "filesystem.h"

FileSystem::FileSystem() {
	for (int i = 0; i < 250; i++) block_map[i] = 0;

	this->initRoot();
}

FileSystem::~FileSystem() {

}

int FileSystem::createFolder(char name[NAME_SIZE], std::string location)
{
	
	return 0;
}

std::string FileSystem::listDir(std::string path)
{
	//Entry entry;
	//readBlock(this->mMemblockDevice.readBlock(0).toString(), &entry);
	//
	//std::string folders;

	return std::string("");
}

void FileSystem::initRoot()
{
	Entry root;
	root.name = "/.\x3";
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
	Block current_block = mMemblockDevice.readBlock(0);
	
	unsigned int curBlockId = 0;
	int i = 0;
	std::string folder = "";
	bool search = true;
	bool foundLocation = false;

	while (search) {
		if (location[i] != '/' && i < location.length()) {
			folder += location[i];
		}
		else {
			

		}
	}
	return curBlockId;
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

void FileSystem::readBlock(std::string block, Entry *folder)
{
	char name[NAME_SIZE];
	
	//name
	int i = 0;
	folder->name = block.substr(0, NAME_SIZE);
	i = NAME_SIZE;

	//blockiD
	std::string id;
	folder->blockId = std::stoi(block.substr(i, 3));
	i += 3;

	//folder
	id = block[i];
	folder->folder = std::stoi(id);
	i++;
		
	//parent
	folder->parent = std::stoi(block.substr(i, 3));
	i += 3;

	//fileSize	
	folder->fileSize = std::stoi(block.substr(i, 8));
	i += 8;

	//link
	folder->link = std::stoi(block.substr(i, 3));
	i += 3;

	//accessRights
	id = block[i];
	folder->accessRights = std::stoi(id);
	i++;

	//data
	folder->data = block.substr(i);

}



/* Please insert your code */



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



