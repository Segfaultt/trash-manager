#include <iostream>//outputting via CL
#include <fstream>//interacting with log files
#include <time.h>//logging time placed and checking current time
#include <boost/filesystem.hpp>//interacting with directories 

bool log(std::string, std::fstream&);

bool isLogged(std::string, std::fstream&);

int main(int argc, const char *argv[])
{
	//check the number of arguments 
	if (argc < 2) {
		std::cout<<"USAGE: trash-manager <trash-directory>\n";
		return -1;
	}

	///////////////////////////////////////////////
	//              Opening logfile              //
	///////////////////////////////////////////////
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
	//opening logfile
	#define LOGFILE (trash.string()+".trash-log")
	bool newLogfile = false;
	std::fstream logfile;
	logfile.open(LOGFILE);
	//if there is no logfile then create one
	if (!logfile.is_open()) {
		std::cout<<"logfile does not exist, creating a new logfile\n";
		//clear iostate flags from original opening attempt 
		logfile.clear();
		//create a new file 
		logfile.open(LOGFILE, std::ios_base::out | std::ios_base::trunc);
		//open it with the needed flags 
		logfile.open(LOGFILE, std::ios_base::out | std::ios_base::out | std::ios_base::trunc);
		//set the newLogfile variable to true
		newLogfile = true;
		//check if it created a logfile
		if (!logfile.is_open()) {
			std::cout<<"fatal error: could not open logfile at "<<LOGFILE<<'\n';
			return -1;
		}
		else {
			std::cout<<"successfully created a logfile at "<<LOGFILE<<'\n';;
		}
	}
	else {
		std::cout<<"successfully opened the existing logfile at "<<LOGFILE<<'\n';;
	}

	//for each file in the trashcan
	for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(trash)) {
		//if it's an existing log file and the file is already logged 
		if (!newLogfile && isLogged(entry.path().string(), logfile)) {
		}
		else {
			log (entry.path().string(), logfile);
		}
	}

	return 0;
}

//add an entry to the log
bool log(std::string file, std::fstream& logFile)
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

bool isLogged(std::string entry, std::fstream& logfile) {
	return false;
}
