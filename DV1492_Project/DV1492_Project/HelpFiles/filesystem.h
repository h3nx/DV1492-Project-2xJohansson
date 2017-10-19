#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>
#include "memblockdevice.h"

#define NAME_SIZE 64
#define BLOCK_ID_SIZE 3
//#define FILE_DATA_SIZE (512 - NAME_SIZE)

class FileSystem
{
private:
    // Here you can add your own data structures
	

	struct Entry {					//nr of chars used
		std::string name;			//64
		unsigned int blockId;		//3
		bool folder;				//1
		unsigned int parent;		//3	
		unsigned int fileSize;		//8
		unsigned int link;			//3
		unsigned int accessRights;	//1
		std::string data;			//429 (remaining in block)
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
	};

	struct DataBlock {
		unsigned int back;			//3
		unsigned int next;			//3
		int reserved;				//2
		std::string data;

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


	std::string load(std::string filePath);
	std::string save(std::string filePath);



private:
	void initRoot();
	int findBlock(std::string location);
	std::string toString(Entry item);
	std::string toString(DataBlock item);
	void readBlock(std::string block, Entry *folder);
	//void readFileBlock(std::string block, File *file);
	void findContentFolder(std::string data, std::vector<int> &ids, std::vector<std::string> &names, std::vector<bool> &folder);

private:
	MemBlockDevice mMemblockDevice;	
	bool block_map[250];
 
	std::string IdToStr(unsigned int id, unsigned int max_size);
};

#endif // FILESYSTEM_H
