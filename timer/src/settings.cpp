#include<algorithm>
#include<string>
#include<fstream>
#include<array>
#include "functions.h"

int ReturnChosenOption();
void ChangeValueOfSettings();
void ChangeChosenOption(char);
std::array<std::string,3> ReturnColors();
std::array<std::string,5> InitializeSettingsArray();
std::string ReturnShowKeysValue();
std::string ReturnInspectionTimeValue();

int ChosenOption = 0;

std::array<std::string, 5> SettingsArray;
std::string colors[5] = {"94", "93", "92", "95", "97"};

std::array<std::string, 5> InitializeSettingsArray(){
	std::ifstream SettingsFile("config/timer.conf");

	int i = 0;
	
	if(!SettingsFile.is_open()){
		std::ofstream CreateSettingsFile("config/timer.conf", std::ofstream::app);
		CreateSettingsFile<<"PB =            95\nNormalSolve =   94\nDuringSolve =   97\nShowKeys =      true\nInspection =    false";
		CreateSettingsFile.close();
		SettingsFile.open("config/timer.conf");
	}
	
	while(SettingsFile.eof()==0 && i<=4){
		getline(SettingsFile, SettingsArray[i]);	
		SettingsArray[i] = SettingsArray[i].substr(16);
		i++;
	}

	SettingsFile.close();
	return SettingsArray;
}

void ChangeValueOfSettings(){
	int NumberLine = 0;

	std::ifstream ReadOldFile("config/timer.conf");
	std::ofstream WriteNewFile("config/ProvisionalTimer.conf");
	
	if(ChosenOption > 2){
		std::string line;
	
		while(ReadOldFile.eof()==0){
			getline(ReadOldFile, line);

			if(NumberLine == ChosenOption){
				if(line.find("true") != std::string::npos) line.replace(line.find("true"), line.length(), "false"); 
				else line.replace(line.find("false"), line.length(), "true");
			}
			WriteNewFile<<line<<"\n";
			NumberLine++;
		}
		rename("config/ProvisionalTimer.conf", "config/timer.conf");
			
		ReadOldFile.close();
		WriteNewFile.close();

		interface("settings");
	}
	else if(ChosenOption <= 2){
		std::string line;
	
		while(ReadOldFile.eof()==0){
			getline(ReadOldFile, line);

			if(NumberLine == ChosenOption){
				std::string* index = std::find(std::begin(colors), std::end(colors), SettingsArray[ChosenOption]);
				
				if(*index != "97") line.replace(line.find(SettingsArray[ChosenOption]), line.length(), colors[distance(colors, index)+1]); 
				else line.replace(line.find(SettingsArray[ChosenOption]), line.length(), "94");
			}
			WriteNewFile<<line<<"\n";
			NumberLine++;	
		}
		
		ReadOldFile.close();
		WriteNewFile.close();

		rename("config/ProvisionalTimer.conf", "config/timer.conf");
	}
}

void ChangeChosenOption(char PlusOrMinus){
	switch(PlusOrMinus){
		case '+': if(ChosenOption < 4) ChosenOption++;
		break;

		case '-': if(ChosenOption > 0) ChosenOption--;
		break;
	}
}

std::array<std::string,3> ReturnColors(){
	if(SettingsArray[0] == "") InitializeSettingsArray();
	return {SettingsArray[0], SettingsArray[1], SettingsArray[2]};
}

std::string ReturnShowKeysValue(){
	if(SettingsArray[0] == "") InitializeSettingsArray();
	return SettingsArray[3];
}

std::string ReturnInspectionTimeValue(){
	if(SettingsArray[0] == "") InitializeSettingsArray();
	return SettingsArray[4];
}

int ReturnChosenOption(){
	return ChosenOption;
}