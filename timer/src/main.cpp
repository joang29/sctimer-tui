#include<thread>
#include "functions.h"

int main(){

	#ifdef __linux__
		system("stty raw");
	#endif

	interface("timer");
	
	std::thread ThreadKeys(keys);
	ThreadKeys.join();
	
	return 0;
}
