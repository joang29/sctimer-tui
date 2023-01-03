#include<iostream>
#include<iomanip>
#include<string>
#include<unistd.h>
#include<thread>
#include<algorithm>
#include<sys/ioctl.h>
#include<fstream>
#include"functions.h"

void timer();
void SaveTimes(int, int, int);
void StartTimer();
void StopTimer();
void ScrambleGenerator();
void options();
bool CheckIfIsNewPB(int, int, int, bool);
void ChangePB(std::array<std::string, 4>);

std::string KeyPressed = "";
bool TimerIsOn = false;
bool ScreenAfterSolve;
int milliseconds=0,seconds=0,minutes=0;

void StartTimer(){
	KeyPressed = " ";
	std::thread ThreadTimer(timer);
	std::thread ThreadStopTimer(StopTimer);
	ThreadTimer.join();
	ThreadStopTimer.join();
}

void timer(){	
	ScreenAfterSolve = false;

	milliseconds=0,seconds=0,minutes=0;
	
	struct winsize WindowSize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
	
	TimerIsOn = true;

	std::array<std::string,3> colors = ReturnColors();
	
	std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f";
	std::cout<<"\033[2K";
	
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

	TimerIsOn = false;
	ScreenAfterSolve = true;

	if(WindowSize.ws_col/2 > 50) std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f"<<std::setw(WindowSize.ws_col/1.55);
	else std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f"<<std::setw(WindowSize.ws_col/1.26);
	ReturnShowKeysValue() == "true" ? std::cout<<"  Save (H)       DNF (F)      +2 (G)" : std::cout<<"  Save           DNF          +2    ";
	
	bool result = CheckIfIsNewPB(milliseconds, seconds, minutes, false);
	
	std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.9);
	std::cout<<"\033[2K";	

	if(minutes >= 1){
		if(seconds <= 9) std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<minutes<<":0"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
		else std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<minutes<<":"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
	
	}
	else std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<seconds<<"."<<milliseconds<<"\033[0m\r"<<std::flush;
}

void ScrambleGenerator(){
	struct winsize WindowSize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
	
	std::string moves[] = {"U", "D", "F", "B", "R", "L"};
	std::string add[] = {"", "'", "2"};
	
	std::string scramble[25];
	int MoveNumber[25];
	
	for(int i = 0; i<25; i++){
		MoveNumber[i] = rand() % 6;
		scramble[i] = moves[MoveNumber[i]];
		
		//avoid move sequences like F2 D2 F2
		if(i > 2 && MoveNumber[i-2] == MoveNumber[i]){
			if(MoveNumber[i]%2==0 && MoveNumber[i]+1 == MoveNumber[i-1]){
				while(MoveNumber[i]+1 == MoveNumber[i-1]){
					MoveNumber[i] = rand() % 6;
					scramble[i] = moves[MoveNumber[i]];
				}
			}
			
			else if(MoveNumber[i]-1 == MoveNumber[i-1]){
				while(MoveNumber[i]-1 == MoveNumber[i+1]){
					MoveNumber[i] = rand() % 6;
					scramble[i] = moves[MoveNumber[i]];
				}
			}
		} 

		//avoid doing the same move twice
		if(i != 0 && scramble[i][0] == scramble[i-1][0]){
			while(scramble[i][0] == scramble[i-1][0]){
				MoveNumber[i] = rand() % 6;
				scramble[i] = moves[MoveNumber[i]];
			}
		}
		scramble[i].append(add[rand() % 3]);
	}
	if(WindowSize.ws_col/2 > 50){
		std::cout<<"\033["<<WindowSize.ws_row/2-3<<";0f"<<std::setw(WindowSize.ws_col/2-30);	
		for(std::string move : scramble) std::cout<<move<<" ";	
	}else{
		std::cout<<"\033["<<WindowSize.ws_row/2-3<<";0f"<<std::setw(WindowSize.ws_col/2-12);
		for(int i = 0; i<25; i++){
			std::cout<<scramble[i]<<" ";
			if(i == 10) std::cout<<"\033["<<WindowSize.ws_row/2-4<<";0f"<<std::setw(WindowSize.ws_col/2-15);
		}
	}
}

void SaveTimes(int milliseconds, int seconds, int minutes){
	std::ofstream TimeFile("timer/config/times.txt", std::ofstream::app);
	if(milliseconds < 10) milliseconds *= 10;
	if(minutes >= 1){
		if(seconds <= 9) TimeFile<<minutes<<":0"<<seconds<<"."<<milliseconds<<"\n";
		else TimeFile<<minutes<<":"<<seconds<<"."<<milliseconds<<"\n";
	}else TimeFile<<seconds<<"."<<milliseconds<<"\n";
}

void options(std::string OptionChosen){
	ScreenAfterSolve = false;
	
	struct winsize WindowSize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);


	if(OptionChosen == "DNF"){
		std::ofstream WriteFile("timer/config/times.txt", std::ofstream::app);	
		WriteFile<<"DNF"<<"\n";

		std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f"<<"\033[2K";
		
		if(WindowSize.ws_col/2 > 50) std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.84);
		else std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.78);
		std::cout<<"\033[2KDNF";
	}
	
	if(OptionChosen == "save"){
		if(milliseconds != 0 || seconds != 0 || minutes != 0){
			SaveTimes(milliseconds, seconds, minutes);
			CheckIfIsNewPB(milliseconds, seconds, minutes, true);
			
			milliseconds = 0, seconds = 0, minutes = 0;

			if(WindowSize.ws_col/2 > 50) std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f"<<std::setw(WindowSize.ws_col/1.8);
			else std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f"<<std::setw(WindowSize.ws_col/1.7);

			std::cout<<"\033[2KSave!";
		}
	}
	
	if(OptionChosen == "+2"){
		std::array<std::string,3> colors = ReturnColors();

		seconds+=2;
		if(seconds >= 60){
			seconds-=60;
			minutes++;
		}

		SaveTimes(milliseconds, seconds, minutes);
		bool result = CheckIfIsNewPB(milliseconds, seconds, minutes, true);

		std::cout<<"\033["<<WindowSize.ws_row/2+2<<";0f"<<"\033[2K";
	
		std::cout<<"\033["<<WindowSize.ws_row/2<<";0f"<<std::setw(WindowSize.ws_col/1.9);
		std::cout<<"\033[2K";

		minutes >= 1 ? std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<minutes<<":"<<seconds<<"."<<milliseconds<<"+" : 
			       std::cout<<"\033[1;"<<colors[result == true ? 0 : 1]<<"m"<<seconds<<"."<<milliseconds<<"+";

		std::cout<<"\033[1;37m";
	}
	
	ScrambleGenerator();
}

void StopTimer(){
	while(KeyPressed == " " && KeyPressed != ""){
		if(getchar()==' ') KeyPressed = "";
		if(ReturnInspectionTimeValue() == "true" && getchar()==' ') KeyPressed = "";
	}
}
bool CheckIfIsNewPB(int milliseconds, int seconds, int minutes, bool IsToSave){
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
		if(!IsToSave) return true;
	}
	
	if(!IsToSave) return false;

	for(int i = 0; i<3; i++){
		if(BestTimes[i+1].find(":") != std::string::npos) BestTimes[i+1].replace(BestTimes[i+1].find(":"),1 , "");
			
		if(BestTimes[i+1] == "--" || averages[i] < stof(BestTimes[i+1])){
			if(averages[i] != 0 && averages[i] != -1){
				BestTimes[i+1] = std::to_string(averages[i]);
				NewBestTime = true;
			}
		}
	}
	
		if(NewBestTime) ChangePB(BestTimes);
		
		return stof(BestTimes[0]) == NewTime ? true : false;

}

bool FunctionTimerIsOn(){
	return TimerIsOn;
}

void ChangeScreenAfterSolveValue(){
	ScreenAfterSolve = false;
}

bool ReturnScreenAfterSolve(){
	return ScreenAfterSolve;
}
