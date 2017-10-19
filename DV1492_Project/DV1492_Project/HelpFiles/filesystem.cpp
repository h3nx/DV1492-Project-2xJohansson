#include "filesystem.h"

FileSystem::FileSystem() {
	for (int i = 0; i < 250; i++) block_map[i] = 0;

	this->initRoot();
}

FileSystem::~FileSystem() {

}

std::string FileSystem::createFile(std::string path)
{
	return this->createEntry(path, 0);
}

std::string FileSystem::createFolder(std::string path)
{	
	return this->createEntry(path, 1);
}

std::string FileSystem::removeFile(std::string path)
{
	//only tested for folders
	return this->remove(path);
}

std::string FileSystem::removeFolder(std::string path)
{
	return this->remove(path);
}


std::string FileSystem::listDir(std::string path)
{
	std::stringstream result;
	result << "Type:\t\tName:\t\t\t\tSize:\n";
	//int blockId = this->findBlock(path);
	int blockId = 0;

	std::string block = this->mMemblockDevice.readBlock(blockId).toString();

	Entry entry;
	this->readBlock(block, &entry);
	
	std::vector<int> ids, sizes; 
	std::vector<std::string> names;
	std::vector<bool> folder;
	findContentFolder(entry.data, ids, names, folder, sizes);

	for (int i = 0; i < ids.size(); i++) {		
		if(folder[i])
			result << "FOLDER";
		else
			result << "FILE";
		result << "\t\t" << names[i];
		
		if (names[i].length() <= 24) {
			int nrOfTabs = 4 - (names[i].length() / 8) % 8;
			for (int j = 0; j < nrOfTabs; j++)
				result << "\t";
		}
		result << sizes[i] << "b" << std::endl;
	}

	return result.str();
}

void FileSystem::initRoot()
{
	Entry root;
	root.name = ".";
	root.blockId = 0;
	root.folder = 1;
	root.parent = 0;
	root.fileSize = 0;
	root.link = 0;
	root.accessRights = 0;
	root.data = "";

	this->writeBlock(0, root.getString());

	/*Entry folder;
	folder.name = "help\x3";
	folder.blockId = 1;
	folder.folder = 1;
	folder.parent = 0;
	folder.fileSize = 0;
	folder.link = 0;
	folder.accessRights = 0;
	folder.data = "002";

	Entry folder2;
	folder2.name = "something\x3";
	folder2.blockId = 2;
	folder2.folder = 1;
	folder2.parent = 1;
	folder2.fileSize = 0;
	folder2.link = 0;
	folder2.accessRights = 0;
	folder2.data = "003";

	Entry folder3;
	folder3.name = "add\x3";
	folder3.blockId = 3;
	folder3.folder = 1;
	folder3.parent = 2;
	folder3.fileSize = 0;
	folder3.link = 0;
	folder3.accessRights = 0;
	folder3.data = "";

	Entry folder4;
	folder4.name = "user\x3";
	folder4.blockId = 4;
	folder4.folder = 1;
	folder4.parent = 0;
	folder4.fileSize = 0;
	folder4.link = 0;
	folder4.accessRights = 0;
	folder4.data = "005";

	Entry folder5;
	folder5.name = "porn\x3";
	folder5.blockId = 5;
	folder5.folder = 1;
	folder5.parent = 4;
	folder5.fileSize = 0;
	folder5.link = 0;
	folder5.accessRights = 0;
	folder5.data = "";

	this->writeBlock(1, folder.getString());
	this->writeBlock(2, folder2.getString());
	this->writeBlock(3, folder3.getString());
	this->writeBlock(4, folder4.getString());
	this->writeBlock(5, folder5.getString());

	Entry test, wtf, kuk, wat;
	test = this->readBlock(0);
	wtf = this->readBlock(3);

	int b1, b2, b3, b4, b5, b6, b7, b8, b9;

	b1 = this->findBlock("./help");
	b2 = this->findBlock("./help/something");
	b3 = this->findBlock("./help/something/add");
	b4 = this->findBlock("./user");
	b5 = this->findBlock("./user/porn");
	b6 = this->findBlock("./wat");
	b7 = this->findBlock("./wat/wat/wat");
	b8 = this->findBlock(".");*/
	//DataBlock nn;
	//nn.setString("00000200asdasdasdasdasdadasdasd");
	//
	//this->writeBlock(1, nn.getString());
	//
	//DataBlock nn2;
	//nn2.setString("00100000cvxcvxcvxcvsdasadasdasdasdasddasdasdadasdasd");
	//
	//this->writeBlock(2, nn2.getString());

}
int FileSystem::findBlock(std::string location)
{
	int blockID = -1;
	std::vector<std::string> path;

	if (location == ".")
		return 0;
	while (location != "")
	{
		int loc = location.find_first_of("/");
		if (loc == -1)
		{
			path.push_back(location);
			location = "";
			break;
		}
		else
			path.push_back(location.substr(0, loc));
		location = location.substr(loc + 1);
	}


	Entry mover = readBlock(0);
	bool found = 0;
	int step = 1;
	int stop = 0;
	while (!found)
	{
		std::vector<unsigned int> adresses = this->parseFolder(mover.data);
		int i = 0;
		while (adresses.size() > i)
		{
			Entry test = readBlock(adresses[i++]);
			if (test.name == path[step])
			{
				step++;
				if (path.size() == step)
					return test.blockId;
				mover = test;
				break;
			}
			if (adresses.size() == i)
				return -1;
		}
		
	}


	return blockID;
}

/*
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
*/
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

FileSystem::Entry FileSystem::readBlock(int block)
{
	Entry toRet;

	std::string data = this->mMemblockDevice.readBlock(block).toString();

	toRet.setString(data);
	int next = toRet.link;
	
	while (next != 0)
	{
		data = this->mMemblockDevice.readBlock(next).toString();
		DataBlock toAdd;
		toAdd.setString(data);
		toRet.additionalData.push_back(toAdd);
		next = toAdd.next;
	}

	return toRet;
}

void FileSystem::findContentFolder(std::string data, std::vector<int> &ids, std::vector<std::string> &names, std::vector<bool> &folder, std::vector<int> &sizes)
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
		sizes.push_back(tmpEntry.fileSize);
		i += 3;
	}
}

std::string FileSystem::remove(std::string path)
{
	int blockNr = this->findBlock(path);
	if (blockNr == -1)
		return std::string("file/folder not found");

	//find block to remove
	std::string entry_str = mMemblockDevice.readBlock(blockNr).toString();
	Entry to_remove;
	this->readBlock(entry_str, &to_remove);

	//if folder make sure it's empty
	if (to_remove.folder) {
		if (to_remove.data != "")
			return std::string("can only remove folders if empty");
	}

	//data to overwrite old block with
	std::string empty_str;
	empty_str.resize(512);
	std::fill(empty_str.begin(), empty_str.end(), '\0');

	//overwrite old block
	if (mMemblockDevice.writeBlock(blockNr, empty_str) == 1) {
		block_map[blockNr] = 0;

		//have to let parent know
		std::string parent_str = mMemblockDevice.readBlock(to_remove.parent).toString();
		Entry parent;
		this->readBlock(parent_str, &parent);	//get parent

		std::vector<int> data;
		std::string id_str;
		int i = 0;
		while (i < parent.data.length()) {		//find our block
			id_str = parent.data.substr(i, 3);
			data.push_back(std::stoi(id_str));
			i += 3;
		}
		for (int i = 0; i < data.size(); i++) {	//remove our block
			if (to_remove.blockId == data[i]) {
				data.erase(data.begin() + i);
				i = data.size();
			}
		}
		id_str = "";							//give back parent block
		for (int i = 0; i < data.size(); i++)
			id_str.append(this->IdToStr(data.at(i), 3));
		parent.data = id_str;


		if (mMemblockDevice.writeBlock(parent.blockId, parent.getString()) == 1) {
			
			if (to_remove.folder) 
				return std::string("folder removed");
			else
				return std::string("file removed");
		}
		//eturn std::string("code error");
	}
	return std::string("failed");
}

std::string FileSystem::createEntry(std::string path, bool folder)
{
	Entry newEntry;

	int freeBlock = 0;
	while (block_map[freeBlock]) {
		freeBlock++;
		if (freeBlock >= 250)
			return "error: no space on disk";
	}
	newEntry.blockId = freeBlock;


	std::string entryName, parentPath;
	int c = path.length();
	while (c >= 0) {
		if (path[c] != '/') {
			entryName += path[c];
			c--;
		}
		else {
			entryName = path.substr(c + 1);
			parentPath = path.substr(0, c);
			break;
		}
	}
	if (entryName.length() > NAME_SIZE)
		return std::string("error: name too long, max=" + std::to_string(NAME_SIZE));
	newEntry.name = entryName;

	int parentId = this->findBlock(parentPath);
	if (parentId == -1)
		return std::string("directory not found");
	newEntry.parent = parentId;

	newEntry.folder = folder;
	newEntry.fileSize = 0;
	newEntry.link = 0;
	newEntry.accessRights = 0;

	std::string parent_str = mMemblockDevice.readBlock(newEntry.parent).toString();
	Entry parent;
	this->readBlock(parent_str, &parent);
	parent.data.append(this->IdToStr(newEntry.blockId, 3));
	this->mMemblockDevice.writeBlock(newEntry.parent, parent.getString());
	//testing
	//parent_str = mMemblockDevice.readBlock(newFolder.parent).toString();
	//this->readBlock(parent_str, &parent);

	if (mMemblockDevice.writeBlock(freeBlock, newEntry.getString()) == 1) {
		this->block_map[freeBlock] = 1;
		if (newEntry.folder)
			return std::string("folder created");
		else
			return std::string("file created");
	}
	else
		return std::string("error: failed writing disk");
}

std::string FileSystem::writeFile(std::string filePath, std::string text)
{
	std::string parent;
	int c = filePath.length();
	while (c >= 0) {
		if (filePath[c] != '/') {
			parent += filePath[c];
			c--;
		}
		else {
			parent = filePath.substr(0, c);
			c = -1;
		}
	}

	int blockId = this->findBlock(filePath);
	if (blockId == -1)
		return std::string("file not found");

	std::string file_str = mMemblockDevice.readBlock(blockId).toString();
	Entry file;
	this->readBlock(file_str, &file);
	file.data.resize(429);		//!!! 512 - attributes size
	if (text.size() > file.data.capacity())
		return ("not enough space on file");
	
	file.data = text;
	if(mMemblockDevice.writeBlock(blockId, file.getString()) == 1)
		return std::string("file written");
	return std::string("failed writing to string");
}

std::string FileSystem::readFile(std::string filePath)
{
	int blockNr = this->findBlock(filePath);
	if (blockNr == -1)
		return std::string("file not found");

	
	std::string file_str = mMemblockDevice.readBlock(blockNr).toString();
	Entry file;
	this->readBlock(file_str, &file);
		
	return file.data;
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

std::vector<unsigned int> FileSystem::parseFolder(std::string data)
{
	std::vector<unsigned int> adress;
	
	while (data != "")
	{
		std::string tmp = data.substr(0, 3);
		data = data.substr(3);
		adress.push_back(std::stoi(tmp));
		if (data.size() < 3)
			break;
	}
	return adress;
}

bool FileSystem::writeBlock(unsigned int blockID, std::string blockData)
{
	if (mMemblockDevice.writeBlock(blockID, blockData) == 1)
	{
		this->block_map[blockID] = 1;
		return true;
	}
	else
		return false;
}




