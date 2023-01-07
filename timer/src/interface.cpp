#include<iostream>
#include<string>
#include<iomanip>
#include<unistd.h>
#include "functions.h"

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include<Windows.h>
#elif __linux__
	#include<sys/ioctl.h>
#endif

std::string ReturnActualInterface();
std::string ActualInterface = "";

void interface(std::string options){
	ChangeScreenAfterSolveValue();


	int NumberOfColumns, NumberOfRows;
	#ifdef _WIN32
		system("cls");

		CONSOLE_SCREEN_BUFFER_INFO ScreenInfo;
    	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ScreenInfo);

		NumberOfColumns = (ScreenInfo.srWindow.Right - ScreenInfo.srWindow.Left+1);
		NumberOfRows = (ScreenInfo.srWindow.Bottom - ScreenInfo.srWindow.Top+1);
			
	#elif __linux__
		system("clear");

		struct winsize WindowSize;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
		NumberOfRows = WindowSize.ws_row;
		NumberOfColumns = WindowSize.ws_col;
	#endif
	std::cout<<"\033[2;0f"<<std::setw(NumberOfColumns/2.1);

	if(ReturnShowKeysValue() == "true"){
		std::cout<<"祥 Timer (Q)     ";
		std::cout<<"  Statics (E)     ";
		std::cout<<"漣 Settings (R)";
		
		std::cout<<"\033["<<NumberOfRows<<";"<<NumberOfColumns-12<<"f";
		
		std::cout<<"  close (X)";
	}else{
		std::cout<<"祥 Timer         ";    
		std::cout<<"  Statics         ";
		std::cout<<"漣 Settings    ";
		
		std::cout<<"\033["<<NumberOfRows<<";"<<NumberOfColumns-12<<"f";
		std::cout<<"  close    ";	
	}
	
	if(options == "timer"){
		ActualInterface = "timer";

		std::cout<<"\033["<<NumberOfRows/2<<";"<<"0f"<<std::setw(NumberOfColumns/2+19);
		std::cout<<"Press space when you start cubing\n";
		
		ScrambleGenerator();
	}else if(options == "statics"){
		ActualInterface = "statics";
	
		std::array<std::string,4> BestStatics = DetermineStatics();

		std::cout<<"\033[8;0f";
	
		std::cout<<"\r\t\tPB: "<<BestStatics[0]<<std::endl;
		std::cout<<"\r\t\tBest Ao5:  "<<BestStatics[1]<<std::endl;
		std::cout<<"\r\t\tBest Ao12: "<<BestStatics[2]<<std::endl;
		std::cout<<"\r\t\tBest Ao50: "<<BestStatics[3]<<"\n\n\n";

		std::array<double, 4> sumAverageArray = SumAverage();
		std::array<std::string, 4> sumAverageArrayString;  

		for(int i = 0; i < 3; i++){
			if(sumAverageArray[i] != 0 && sumAverageArray[i] != -1){
				sumAverageArray[i] = NumberToTime(sumAverageArray[i]);

				std::stringstream ss;
				ss<<std::fixed<<std::setprecision(2)<<sumAverageArray[i];
				sumAverageArrayString[i] = ss.str();

				if(sumAverageArray[i] >= 1000)	sumAverageArrayString[i].insert(2,":");					
				else if(sumAverageArray[i] >= 100) sumAverageArrayString[i].insert(1,":");
				
			}else if(sumAverageArray[i] == 0) sumAverageArrayString[i] = "--";
			else if(sumAverageArray[i] == -1) sumAverageArrayString[i] = "DNF";
	}
	
		std::cout<<"\r\t\tActual Ao5:   "<<sumAverageArrayString[0]<<std::endl;		
		std::cout<<"\r\t\tActual Ao12:   "<<sumAverageArrayString[1]<<std::endl;
		std::cout<<"\r\t\tActual Ao50:   "<<sumAverageArrayString[2]<<std::endl;

	}else if(options == "settings"){
		ActualInterface = "settings";

		int ChosenOption = ReturnChosenOption();
		std::string MessageSettingsArray[5];
		
		std::array<std::string, 5> SettingsArray = InitializeSettingsArray();
	
		MessageSettingsArray[0] = "[\033[" + SettingsArray[0] + "mﱢ" + "\033[0m" + "] Timer color when you do your PB";
		MessageSettingsArray[1] = "[\033[" + SettingsArray[1] + "mﱢ" + "\033[0m" + "] Timer color at the end of the solve";
		MessageSettingsArray[2] = "[\033[" + SettingsArray[2] + "mﱢ" + "\033[0m" + "] Timer color during normal solve";
		MessageSettingsArray[3] = SettingsArray[3] == "true" ? "[X] Show keys" :  "[ ] Show keys";
		MessageSettingsArray[4] = SettingsArray[4] == "true" ? "[X] Inspection time" :  "[ ] Inspection time"; 
		
		std::cout<<"\033[8;0f";

		for(int i = 0; i<=4; i++) 
			ChosenOption == i ? std::cout<<"\r\t      > "<<MessageSettingsArray[i]<<std::endl : std::cout<<"\r\t\t"<<MessageSettingsArray[i]<<std::endl;
		
		std::cout<<"\033["<<NumberOfRows<<";0f";

		std::cout<<"\rUse 'W' and 'S' to move through the settings and space to change settings";
	}
}

std::string ReturnActualInterface(){
	return ActualInterface;
}
