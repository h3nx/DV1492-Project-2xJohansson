#ifndef FILESYSTEM_H
#define FILESYSTEM_H

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
	};

	struct DataBlock {
		unsigned int back;			//3
		unsigned int next;			//3
		int reserved;				//2
		std::string data;			
	};
								
	/*
	
	struct File {					//nr of chars used
		char name[NAME_SIZE];		//64
		unsigned int blockId;		//3
		//Attributes atts;			//64
		char data[368];				//360 (remaining in block)
		
		//File* link;//8 (behövs om filen ska vara flera block)
	};

	struct Folder {		
		char name[NAME_SIZE];		//64
		unsigned int blockId;		//3
		
		unsigned int parent;		//3
		
		unsigned int link;			//3

		//unsigned int nrOfFolders;	//8
		//unsigned int nrOfFiles;		//0
		unsigned int itemIds[145];	//436/3 (remaining in block)


		//unsigned int nrOfFolders;	//4
		//unsigned int *folders;

	/*	Folder() : name(),
			blockId(0),
			parent(0),
			link(0),
			nrOfFolders(0),
			nrOfFiles(0),
			itemIds() {}
			//folders(nullptr), 
			//nrOfFolders(0){}
			~Folder() {	}
	};
	*/


public:
    FileSystem();
    ~FileSystem();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

    /* This function creates a file in the filesystem */
	std::string createFile(std::string path);

    /* Creates a folder in the filesystem */
    int createFolder(char name[NAME_SIZE], std::string location);

    /* Removes a file in the filesystem */
    std::string removeFile(std::string path);

    /* Removes a folder in the filesystem */
    std::string removeFolder(std::string path);

    /* Function will move the current location to a specified location in the filesystem */
    std::string goToFolder(std::string path);

    /* This function will get all the files and folders in the specified folder */
    std::string listDir(std::string path);

    /* Add your own member-functions if needed */
private:
	void initRoot();
	int findBlock(std::string location);
	std::string toString(Entry item);
	std::string toString(DataBlock item);
	void readBlock(std::string block, Entry *folder);
	//void readFileBlock(std::string block, File *file);

private:
	MemBlockDevice mMemblockDevice;	
	bool block_map[250];
};

#endif // FILESYSTEM_H
