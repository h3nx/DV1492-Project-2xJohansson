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
	Folder root;
	root.name[0] = 'r';
	root.name[1] = 'o';
	root.name[2] = 'o';
	root.name[3] = 't';
	root.name[4] = '\n';
	root.blockId = 123;
	root.parent = 0;
	root.link = 0;
	root.files = nullptr;
	root.nrOfFiles = 0;


	mMemblockDevice.writeBlock(0, this->toString(root));
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

std::string FileSystem::toString(Folder item){
	std::vector<char> tmp;
	int i = 0;
		
	//name		
	for (int j = 0; j < NAME_SIZE; j++) {
		tmp.push_back(item.name[j]);
		i++;
	}

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
std::string FileSystem::toString(File item) {
	return "0";
}

void FileSystem::readFolderBlock(std::string block, Folder *folder)
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

void FileSystem::readFileBlock(std::string block, File *file)
{

}


/* Please insert your code */