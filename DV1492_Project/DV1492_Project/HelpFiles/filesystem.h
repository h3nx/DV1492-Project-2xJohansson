#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"

#define NAME_SIZE 64
//#define FILE_DATA_SIZE (512 - NAME_SIZE)

class FileSystem
{
private:
    // Here you can add your own data structures
	
	struct Attributes {		
		char owner[NAME_SIZE];
		unsigned int curSize;
		unsigned int maxSize;
	};//64+8+8 = 80
	
	struct File {					//BYTE SIZE
		char name[NAME_SIZE];				//64
		unsigned int blockId;		//8
		Attributes atts;			//80
		char data[368];				//360 (remaining in block)
		
		//File* link;//8 (behövs om filen ska vara flera block)
	};

	struct Folder {		
		char name[NAME_SIZE];				//64
		unsigned int blockId;		//8
		//Folder* parent;				//8
		unsigned int parent;		//8
		//Folder* link;				//8
		unsigned int link;		//8

		//File** files;				//nrOfFiles * 8
		unsigned int *files;
		unsigned int nrOfFiles;		//4
		//Folder** folders;			//nrOfFolders * 8
		unsigned int *folders;
		unsigned int nrOfFolders;	//4

		Folder() : name(), 
			blockId(0),
			parent(0), 
			link(0), 
			files(nullptr), 
			nrOfFiles(0), 
			folders(nullptr), 
			nrOfFolders(0){}
		~Folder() {	delete files; delete folders; }
					
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
	std::string toString(Folder item);
	std::string toString(File item);
	void readFolderBlock(std::string block, Folder *folder);
	void readFileBlock(std::string block, File *file);

private:
	MemBlockDevice mMemblockDevice;	
	bool block_map[250];
};

#endif // FILESYSTEM_H
