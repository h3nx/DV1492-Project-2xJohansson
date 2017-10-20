#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>
#include <sstream>
#include <algorithm>
#include "memblockdevice.h"

#define NAME_SIZE 64
#define BLOCK_ID_SIZE 3
//#define FILE_DATA_SIZE (512 - NAME_SIZE)

class FileSystem
{
private:
    // Here you can add your own data structures
	struct DataBlock;
	struct Entry;

	struct Entry {					//nr of chars used
		std::string name = "";			//64
		unsigned int blockId = -1;		//3
		bool folder = 0;				//1
		unsigned int parent = 000;		//3	
		unsigned int fileSize = 0;		//8
		unsigned int link = 0;			//3
		unsigned int accessRights = 0;	//1
		std::string data = "";			//429 (remaining in block)
		std::string getString()
		{
			std::string result = "";
			std::string base = "00000000";
			std::string tmp = "";
			result += this->name;
			for (int i = this->name.size(); i < NAME_SIZE; i++)
				result += '\0';

			tmp = base + std::to_string(this->blockId);
			result += tmp.substr(tmp.size() - 3);

			result += std::to_string(this->folder);

			tmp = base + std::to_string(this->parent);
			result += tmp.substr(tmp.size() - 3);

			tmp = base + std::to_string(this->fileSize);
			result += tmp.substr(tmp.size() - 8);

			tmp = base + std::to_string(this->link);
			result += tmp.substr(tmp.size() - 3);

			result += std::to_string(this->accessRights);

			result += this->data;
			int remaining = 512 - result.length();
			for (int p = 0; p < remaining; p++)
				result += '\0';

			return result;
		}
		void setString(std::string input)
		{
			char name[NAME_SIZE];

			//name
			int i = 0;
			this->name = input.substr(0, NAME_SIZE);
			this->name.erase(std::remove(this->name.begin(), this->name.end(), '\0'), this->name.end());
			i = NAME_SIZE;

			//blockiD
			std::string id;
			this->blockId = std::stoi(input.substr(i, 3));
			i += 3;

			//folder
			id = input[i];
			this->folder = std::stoi(id);
			i++;

			//parent
			this->parent = std::stoi(input.substr(i, 3));
			i += 3;

			//fileSize	
			this->fileSize = std::stoi(input.substr(i, 8));
			i += 8;

			//link
			this->link = std::stoi(input.substr(i, 3));
			i += 3;

			//accessRights
			id = input[i];
			this->accessRights = std::stoi(id);
			i++;

			//data
			this->data = input.substr(i);
			this->data.erase(std::remove(this->data.begin(), this->data.end(), '\0'), this->data.end());
		}

		std::vector<DataBlock> additionalData;
	};

	struct DataBlock {
		unsigned int back = 0;			//3
		unsigned int next = 0;			//3
		int reserved = 0;				//2
		std::string data = "";
		std::string getString()
		{
			std::string result = "";
			std::string base = "00000000";
			std::string tmp = "";

			tmp = base + std::to_string(this->back);
			result += tmp.substr(tmp.size() - 3);
			tmp = base + std::to_string(this->next);
			result += tmp.substr(tmp.size() - 3);
			tmp = base + std::to_string(this->reserved);
			result += tmp.substr(tmp.size() - 2);

			result += this->data;
			int remaining = 512 - result.length();
			for (int p = 0; p < remaining; p++)
				result += '\0';


			return result;
		}
		void setString(std::string input)
		{
			int i = 0;
			this->back = std::stoi(input.substr(i, 3));
			i += 3;
			this->next = std::stoi(input.substr(i, 3));
			i += 3;
			this->reserved = std::stoi(input.substr(i, 3));
			i += 2;


			this->data = input.substr(i);
			this->data.erase(std::remove(this->data.begin(), this->data.end(), '\0'), this->data.end());
		}
	

	};
	

public:
    FileSystem();
    ~FileSystem();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
	std::string createFile(std::string path);

    /* Creates a folder in the filesystem */
    std::string createFolder(std::string path);

    /* Removes a file in the filesystem */
    std::string removeFile(std::string path);

    /* Removes a folder in the filesystem */
	std::string removeFolder(std::string path);

    /* Function will move the current location to a specified location in the filesystem */
    std::string goToFolder(std::string path);

    /* This function will get all the files and folders in the specified folder */
    std::string listDir(std::string path);

    /* Add your own member-functions if needed */
	std::string writeFile(std::string filePath, std::string text);
	std::string readFile(std::string filePath);

	std::string load(std::string filePath);
	std::string save(std::string filePath);



private:
	void initRoot();
	int findBlock(std::string location);
	void findContentFolder(std::string data, std::vector<int> &ids, std::vector<std::string> &names, std::vector<bool> &folder, std::vector<int> &sizes);
	std::string remove(std::string path);
	std::string createEntry(std::string path, bool folder);
	FileSystem::Entry readBlock(int block);
	std::string IdToStr(unsigned int id, unsigned int max_size);
	std::vector<unsigned int> parseFolder(std::string data);
	bool writeBlock(unsigned int blockID, std::string blockData);

private:
	MemBlockDevice mMemblockDevice;	
	bool block_map[250];
 
};

#endif // FILESYSTEM_H
