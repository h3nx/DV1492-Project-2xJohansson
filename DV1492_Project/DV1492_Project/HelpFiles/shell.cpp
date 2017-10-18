#include <iostream>
#include <sstream>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};
FileSystem* mFileSys;




/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit();
std::string help();

/* More functions ... */

std::string format();
std::string ls(std::string path);
std::string create(std::string path);
std::string cat(std::string path);
std::string createImage(std::string path);
std::string restoreImage(std::string path);
std::string rm(std::string path);
std::string cp(std::string source, std::string destination);
std::string append(std::string source, std::string destination);
std::string mv(std::string oldFile, std::string newFile);
std::string mkDir(std::string directory);
std::string cd(std::string directory);
std::string pwd();


int main(void) {

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "Johansson@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

    bool bRun = true;
	mFileSys = new FileSystem();

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

			case 0: //quit
				bRun = quit();                
                break;
            case 1: // format
				std::cout << format() << std::endl;
                break;
            case 2: // ls
                std::cout << "Listing directory" << ls(currentDir) << std::endl;
                break;
            case 3: // create
				std::cout << create(commandArr[1]) << std::endl;
                break;
            case 4: // cat
				std::cout << cat(commandArr[1]) << std::endl;
                break;
            case 5: // createImage
				std::cout << createImage(commandArr[1]) << std::endl;
                break;
            case 6: // restoreImage
				std::cout << restoreImage(commandArr[1]) << std::endl;
                break;
            case 7: // rm
				std::cout << rm(commandArr[1]) << std::endl;
                break;
            case 8: // cp
				std::cout << cp(commandArr[1], commandArr[2]) << std::endl;
                break;
            case 9: // append
				//std::cout << append() << std::endl;
                break;
            case 10: // mv
				//std::cout << mv() << std::endl;
                break;
            case 11: // mkdir
				std::cout << mkDir(commandArr[1]) << std::endl;
                break;
            case 12: // cd
				std::cout << cd(commandArr[1]) << std::endl;
                break;
            case 13: // pwd
				std::cout << pwd() << std::endl;
                break;
            case 14: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

bool quit() {
	std::cout << "Exiting\n";
	return false;
}
std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}
/* Insert code for your shell functions and call them from the switch-case */

std::string format()
{
	std::string response = "Disk formatted";
	delete mFileSys;
	mFileSys = new FileSystem();
	return response;
}
std::string ls(std::string path)
{	
	return mFileSys->listDir(path);
}
std::string create(std::string path)
{
	std::string response = "";

	return response;
}
std::string cat(std::string path)
{
	std::string response = "";

	return response;
}
std::string createImage(std::string path)
{
	return mFileSys->save(path);
}
std::string restoreImage(std::string path)
{
	return mFileSys->load(path);
}
std::string rm(std::string path)
{
	std::string response = "";

	return response;
}
std::string cp(std::string source, std::string destination)
{
	std::string response = "";

	return response;
}
std::string append(std::string source, std::string destination)
{
	std::string response = "";

	return response;
}
std::string mv(std::string oldFile, std::string newFile)
{
	std::string response = "";

	return response;
}
std::string mkDir(std::string directory)
{
	return mFileSys->createFolder(directory);	
}
std::string cd(std::string directory)
{
	std::string response = "";

	return response;
}
std::string pwd()
{
	std::string response = "";

	return response;
}









