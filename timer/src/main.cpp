#include<thread>
#include<sys/stat.h>
#include<sys/types.h>
#include "functions.h"

int main(){
	struct stat CheckIfDirExists;
	
	#ifdef __linux__
		system("stty raw");
		std::string homedir = getenv("HOME");

		if(stat((homedir + "/.config/sctimer-cli").c_str(), &CheckIfDirExists) != 0){
 			mkdir((homedir + "/.config/sctimer-cli").c_str(), 0777);
			mkdir((homedir + "/.config/sctimer-cli/2x2").c_str(), 0777);
			mkdir((homedir + "/.config/sctimer-cli/3x3").c_str(), 0777);
		}		
	#elif _WIN32
		std::string appdata = getenv("APPDATA");
    		if(stat((appdata + "/../Local/sctimer-cli").c_str(), &CheckIfDirExists) != 0){
	
			mkdir((appdata + "/../Local/sctimer-cli").c_str());
			mkdir((appdata + "/../Local/sctimer-cli/2x2").c_str());
			mkdir((appdata + "/../Local/sctimer-cli/3x3").c_str());
		}
	#endif
	
	interface("timer");
	
	std::thread ThreadKeys(keys);
	ThreadKeys.join();
	
	return 0;
}
