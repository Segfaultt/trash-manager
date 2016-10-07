#include <iostream>//outputting via CL
#include <fstream>//interacting with log files
#include <time.h>//logging time placed and checking current time
#include <boost/filesystem.hpp>//interacting with directories 

bool log(std::string, std::fstream&);

bool isLogged(std::string, std::fstream&);

int main(int argc, const char *argv[])
{
	if (argc < 2) {//check the number of arguments 
		std::cout<<"USAGE: trash-manager <trash-directory>\n";
		return -1;
	}

	///////////////////////////////////////////////
	//              Opening logfile              //
	///////////////////////////////////////////////

	boost::filesystem::path trash(argv[1]);//open trashcan 

	if (exists(trash)) {//check if trashcan exists
		if (!is_directory(trash)) {//and is also a directory
			std::cout<<"fatal error: specified trashcan exists but isn't a directory\n";
			return -1;
		}
	}
	else {
		std::cout<<"fatal error: specified trashcan does not exist\n";
		return -1;
	}
	#define LOGFILE (trash.string()+".trash-log")
	bool newLogfile = false;
	std::fstream logfile;
	logfile.open(LOGFILE, std::ios_base::in | std::ios_base::out | std::ios_base::app);

	if (!logfile.is_open()) {//if there is no logfile then create one
		std::cout<<"logfile does not exist, creating a new logfile\n";
		logfile.clear();//clear iostate flags from original opening attempt 
		logfile.open(LOGFILE, std::ios_base::out);//create a new file (no need for input)
		newLogfile = true;
		if (!logfile.is_open()) {//check if it created a logfile successfully 
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

	///////////////////////////////////////////////////
	//            Logging/deleting files             //
	///////////////////////////////////////////////////
	for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(trash)) {//for each file in the trashcan
		logfile.clear();
		if (!newLogfile && isLogged(entry.path().string(), logfile)) {//if it's an existing log file and the file is already logged 
			std::cout<<"entry \""<<entry.path().string()<<"\" is already logged\n";
		}
		else if (entry.path().filename().string() != ".trash-log" && entry.path().filename().string() != ".DS_Store" ) {
			log (entry.path().string(), logfile);
		}
	}

	return 0;
}

//add an entry to the log
bool log(std::string file, std::fstream& logfile)
{
	if (logfile<<file<<'\n'<<time(NULL)<<'\n') { //if it successfully wrote to the file
		std::cout<<"successfully logged entry \""<<file<<"\"\n";
		return true;
	}
	else {
		std::cout<<"failed logging entry \""<<file<<"\"\n";
		return false;
	}
}

//check if an entry is logged
bool isLogged(std::string entry, std::fstream& logfile) {
	int oldPosition = logfile.tellg();//get the position before search
	logfile.seekg(std::ios_base::beg);//start search from beggining of the file
	std::string testString;
	for (int i = 0; logfile.peek() != EOF; i++) {
		getline(logfile, testString);
		logfile.ignore(2048, '\n');//drop a line for the time logged
		if (entry == testString) {//if it's what we're looking for
			return true;
		}
	}
	logfile.seekg(oldPosition);//return to the position from before the search
	return false;
}
