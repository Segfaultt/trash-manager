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
	logfile.open(LOGFILE, std::ios_base::in | std::ios_base::out );
	//if there is no logfile then create one
	if (!logfile.is_open()) {
		std::cout<<"logfile does not exist, creating a new logfile\n";
		//clear iostate flags from original opening attempt 
		logfile.clear();
		//create a new file 
		logfile.open(LOGFILE, std::ios_base::out);// | std::ios_base::app);
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

	///////////////////////////////////////////////////
	//            Logging/deleting files             //
	///////////////////////////////////////////////////
	//for each file in the trashcan
	for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(trash)) {
		//if it's an existing log file and the file is already logged 
		logfile.clear();
		if (!newLogfile && isLogged(entry.path().string(), logfile)) {
			std::cout<<"entry \""<<entry.path().string()<<"\" is already logged\n";
		}
		else {
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
	logfile.seekg(std::ios_base::beg);
	std::string testString;
	for (int i = 0; logfile.peek() != EOF; i++) {
		getline(logfile, testString);
		if (entry == testString) {
			return true;
		}
		else {
			logfile.ignore(2048, '\n');
		}
	}
	return false;
}
