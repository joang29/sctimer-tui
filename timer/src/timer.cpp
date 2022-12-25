#include<iostream>
#include<iomanip>
#include<string>
#include<unistd.h>
#include<thread>
#include<sys/ioctl.h>
#include<fstream>
#include"functions.h"

void timer();
void SaveTimes(int, int, int);
void StartTimer();
void StopTimer();
void ScrambleGenerator();
bool IsNewPB(int, int, int);
void ChangePB(std::array<std::string, 4>);

std::string KeyPressed = "";
bool TimerIsOn = false;

void StartTimer(){
	KeyPressed = " ";
	std::thread ThreadTimer(timer);
	std::thread ThreadStopTimer(StopTimer);
	ThreadTimer.join();
	ThreadStopTimer.join();
}

void timer(){	
	int milliseconds=0,seconds=0,minutes=0;
	
	struct winsize WindowSize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
	
	TimerIsOn = true;

	std::array<std::string,3> colors = ReturnColors();
	
	if(ReturnInspectionTimeValue() == "true"){
		seconds = 15;

		while(seconds > 0 && seconds <= 15 && KeyPressed == " "){		
			if(milliseconds == 0){
				seconds--;
				milliseconds+=100;
			}

			std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.9);
			std::cout<<"\033[2K";
			
			std::cout<<"\033[1;"<<colors[2]<<"m"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;

			milliseconds--;
			
			usleep(10000);
		}
		KeyPressed = " ";
		seconds = 0;
		milliseconds = 0;
	}

	while(KeyPressed == " "){
		milliseconds++;

		if(milliseconds == 100){
			seconds++;
			milliseconds-=100;
		}else if(seconds == 60){
			minutes++;
			seconds-=60;
		}
		
		std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.9);
		std::cout<<"\033[2K";	

		if(minutes >= 1){
			if(seconds <= 9) std::cout<<"\033[1;"<<colors[2]<<"m"<<minutes<<":0"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
			else std::cout<<"\033[1;"<<colors[2]<<"m"<<minutes<<":"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
		}
		else std::cout<<"\033[1;"<<colors[2]<<"m"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
		
		usleep(10000);
	}

	std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.9);
	std::cout<<"\033[2K";	
	
	SaveTimes(milliseconds, seconds, minutes);
	bool result = IsNewPB(milliseconds, seconds, minutes);
	
	if(minutes >= 1){
		if(seconds <= 9) std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<minutes<<":0"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
		else std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<minutes<<":"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
	
	}
	else std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;

	TimerIsOn = false;
	ScrambleGenerator();
}

void ScrambleGenerator(){
	struct winsize WindowSize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
	
	std::string moves[] = {"U", "U'", "U2", "D", "D'", "D2", "B", "B'", "B2", "F", "F'", "F2", "R", "R'", "R2", "L", "L'", "L2"};
	std::string scramble[25];
	
	if(WindowSize.ws_col/2 > 50){
		std::cout<<"\033["<<WindowSize.ws_row/2-3<<";0f"<<std::setw(WindowSize.ws_col/2-30);	
		for(int i = 0; i<25; i++){
			std::cout<<moves[rand() % 18]<<" ";
		}	
	}else{
		std::cout<<"\033["<<WindowSize.ws_row/2-3<<";0f"<<std::setw(WindowSize.ws_col/2-12);
		for(int i = 0; i<25; i++){
			std::cout<<moves[rand() % 18]<<" ";
				if(i == 10) std::cout<<"\033["<<WindowSize.ws_row/2-4<<";0f"<<std::setw(WindowSize.ws_col/2-15);
		}
	}
}

void SaveTimes(int milliseconds, int seconds, int minutes){
	std::ofstream TimeFile("config/times.txt", std::ofstream::app);
	if(milliseconds < 10) milliseconds *= 10;
	if(minutes >= 1){
		if(seconds <= 9) TimeFile<<minutes<<":0"<<seconds<<"."<<milliseconds<<"\n";
		else TimeFile<<minutes<<":"<<seconds<<"."<<milliseconds<<"\n";
	}else TimeFile<<seconds<<"."<<milliseconds<<"\n";
}

bool FunctionTimerIsOn(){
	return TimerIsOn;
}

void StopTimer(){
	while(KeyPressed == " " && KeyPressed != ""){
		if(getchar()==' '){
			KeyPressed = "";
		}
		if(ReturnInspectionTimeValue() == "true" && getchar()==' '){
			KeyPressed = "";
		}
	}
}
bool IsNewPB(int milliseconds, int seconds, int minutes){
	std::array<std::string,4> BestTimes = DetermineStatics();
	std::array<double, 4> averages = SumAverage();
	float NewTime;
	
	if(minutes >= 1 && seconds <= 9) NewTime = stof(std::to_string(minutes) + '0' + std::to_string(seconds) + '.' + std::to_string(milliseconds));
	else NewTime = stof(std::to_string(minutes) + std::to_string(seconds) + '.' + std::to_string(milliseconds));
	
	bool NewBestTime = false;
	if(BestTimes[0].find(":") != std::string::npos) BestTimes[0].replace(BestTimes[0].find(":"),1 , "");
	
	if(BestTimes[0] == "--" || NewTime < stof(BestTimes[0])){
		BestTimes[0] = std::to_string(NewTime);
		NewBestTime = true;
	}

	for(int i = 0; i<3; i++){
		if(BestTimes[i+1].find(":") != std::string::npos) BestTimes[i+1].replace(BestTimes[i+1].find(":"),1 , "");
		
		if(BestTimes[i+1] == "--" || averages[i] < stof(BestTimes[i+1])){
			if(averages[i] != 0){
				BestTimes[i+1] = std::to_string(averages[i]);
				NewBestTime = true;
			}
		}
	}
	
	if(NewBestTime) ChangePB(BestTimes);

	return stof(BestTimes[0]) == NewTime ? true : false;
}
