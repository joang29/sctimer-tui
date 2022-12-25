#include<thread>
#include "functions.h"

int main(){
	system("stty raw");

	interface("timer");
	
	std::thread ThreadKeys(keys);
	ThreadKeys.join();
	
	return 0;
}
