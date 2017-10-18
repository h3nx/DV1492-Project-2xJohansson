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


void FileSystem::initRoot()
{
	Entry root;
	root.name = "/.\n";
	root.blockId = 0;
	root.folder = 1;
	root.parent = 0;
	root.fileSize = 0;
	root.link = 0;
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
	std::string tmp;
	int i = 0;
		
	//name		
	tmp += item.name;
	
	//block id		
	char num[3] = { 0, 0, 0 };
	num[2] = (item.blockId % 100) % 10;
	num[1] = ((item.blockId - num[2]) % 100) / 10;
	num[0] = (item.blockId - num[2] - num[1]*10) / 100;
	
	for (int j = 0; j < 3; j++)
		tmp.push_back(num[j]);
	i += 3;

	//fill rest with 0 (temp)
	while (i < 512) {
		tmp.push_back('0');
		i++;
	}
	
	return std::string(tmp.begin(), tmp.end());
}
std::string FileSystem::toString(DataBlock item) {
	return "0";
}

void FileSystem::readBlock(std::string block, Entry *folder)
{
	char name[NAME_SIZE];
	int i = 0;
	
	//get name
	if (i < NAME_SIZE) {
		folder->name[i] = block[i];
		i++;
	}
	//get blockiD
	for (int i = 0; i < 3; i++) {
		std::string id = { block[i], block[i + 1], block[i + 2] };
		std::string::size_type sz;
		folder->blockId = std::stoi(id, &sz);
	}

	
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