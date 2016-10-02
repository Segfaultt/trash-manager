#include <iostream>//outputting via CL
#include <fstream>//interacting with log files
#include <time.h>//logging time placed and checking current time
#include <boost/filesystem.hpp>//interacting with directories 

bool log(std::string, std::fstream&, int&);

int main(int argc, const char *argv[])
{
	//check the number of arguments 
	if (argc < 2) {
		std::cout<<"USAGE: trash-manager <trash-directory>\n";
		return -1;
	}

	//declare trashcan directory
	boost::filesystem::path trash(argv[1]);

	//check if trashcan exists and is a directory
	if (exists(trash)) {
		if (!is_directory(trash)) {
			std::cout<<"fatal error: specified trashcan exists but isn't a directory\n";
			return -1;
		}
	}
	else {
		std::cout<<"fatal error: specified trashcan does not exist\n";
		return -1;
	}

	

	return 0;
}

//add an entry to the log
bool log(std::string file, std::fstream& logFile, int& entries)
{
	logFile<<file<<' '<<time(NULL)<<'\n';

	//return true if unsuccessful
	if (logFile.fail()) {
		//output error message
		if (!logFile.is_open()) {
			std::cout<<"error: log file not open\n";
		}
		else if (logFile.eof()) {
			std::cout<<"error: log file reached EOF\n";
		}
		else {
			std::cout<<"error: writing to log file failed\n";
		}
		return true;
	}
	//if there are no errors return false
	return false;
}
