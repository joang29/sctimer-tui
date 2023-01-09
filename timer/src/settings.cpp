#include<algorithm>
#include<string>
#include<fstream>
#include<array>
#include "functions.h"

#include<iostream>

int ReturnChosenOption();
void ChangeValueOfSettings();
void ChangeChosenOption(char);
std::array<std::string,3> ReturnColors();
std::array<std::string,6> InitializeSettingsArray();
std::string ReturnShowKeysValue();
std::string ReturnInspectionTimeValue();
std::string ReturnCube();

int ChosenOption = 0;

std::array<std::string, 6> SettingsArray;
std::string colors[5] = {"94", "93", "92", "95", "97"};

std::array<std::string, 6> InitializeSettingsArray(){
	#ifdef _WIN32
		std::string ConfigDirectoryPath = std::string(getenv("APPDATA")) + "/../Local/sctimer-cli/";
	#elif __linux__
		std::string ConfigDirectoryPath = std::string(getenv("HOME")) + "/.config/sctimer-cli/";
	#endif

	std::ifstream SettingsFile(ConfigDirectoryPath + "timer.conf");

	int i = 0;
	
	if(!SettingsFile.is_open()){
		std::ofstream CreateSettingsFile(ConfigDirectoryPath + "timer.conf", std::ofstream::app);
		CreateSettingsFile<<"Cube =          3x3\nPB =            95\nNormalSolve =   94\nDuringSolve =   97\nShowKeys =      true\nInspection =    false";
		
		CreateSettingsFile.close();
		SettingsFile.close();
		
		SettingsArray = {"3x3", "95", "94", "97", "true", "false"};

		return SettingsArray;		
	}
	
	while(SettingsFile.eof()==0 && i<=5){
		getline(SettingsFile, SettingsArray[i]);	
		SettingsArray[i] = SettingsArray[i].substr(16);
		i++;
	}

	SettingsFile.close();
	return SettingsArray;
}

void ChangeValueOfSettings(){
	#ifdef _WIN32
		std::string ConfigDirectoryPath = std::string(getenv("APPDATA")) + "/../Local/sctimer-cli/";
	#elif __linux__
		std::string ConfigDirectoryPath = std::string(getenv("HOME")) + "/.config/sctimer-cli/";
	#endif

	int NumberLine = 0;
	std::string line;
	
	std::ifstream ReadOldFile(ConfigDirectoryPath + "timer.conf");
	std::ofstream WriteNewFile(ConfigDirectoryPath + "ProvisionalTimer.conf");
	
	if(ChosenOption == 0){
		while(ReadOldFile.eof()==0){
			getline(ReadOldFile, line);
			if(NumberLine == 0)
				line.find("3x3") != std::string::npos ? line.replace(line.find("3x3"), line.length(), "2x2") : line.replace(line.find("2x2"), line.length(), "3x3");
			if(line != "") WriteNewFile<<line<<"\n";
			NumberLine++;
		}
	}
	else if(ChosenOption > 3){	
		while(ReadOldFile.eof()==0){
			getline(ReadOldFile, line);

			if(NumberLine == ChosenOption){
				line.find("true") != std::string::npos ? line.replace(line.find("true"), line.length(), "false") : 
									 line.replace(line.find("false"), line.length(), "true");
			}
			if(line != "") WriteNewFile<<line<<"\n";
			NumberLine++;
		}
	}
	else if(ChosenOption > 0 && ChosenOption <= 3){
		while(ReadOldFile.eof()==0){
			getline(ReadOldFile, line);

			if(NumberLine == ChosenOption){
				std::string* index = std::find(std::begin(colors), std::end(colors), SettingsArray[ChosenOption]);
				
				if(*index != "97") line.replace(line.find(SettingsArray[ChosenOption]), line.length(), colors[distance(colors, index)+1]); 
				else line.replace(line.find(SettingsArray[ChosenOption]), line.length(), "94");
			}
			if(line != "") WriteNewFile<<line<<"\n";
			NumberLine++;	
		}
	}
		
		ReadOldFile.close();
		WriteNewFile.close();	

		#ifdef _WIN32
			remove((ConfigDirectoryPath + "timer.conf").c_str());
			rename((ConfigDirectoryPath + "ProvisionalTimer.conf").c_str(), (ConfigDirectoryPath + "timer.conf").c_str());
	
		#elif __linux__
			rename((ConfigDirectoryPath + "ProvisionalTimer.conf").c_str(), (ConfigDirectoryPath + "timer.conf").c_str());
		#endif

		interface("settings");
}

void ChangeChosenOption(char PlusOrMinus){
	switch(PlusOrMinus){
		case '+': if(ChosenOption < 5) ChosenOption++;
		break;

		case '-': if(ChosenOption > 0) ChosenOption--;
		break;
	}
}


std::string ReturnCube(){
	if(SettingsArray[0] == "") InitializeSettingsArray();
	return SettingsArray[0];
}

std::array<std::string,3> ReturnColors(){
	if(SettingsArray[1] == "") InitializeSettingsArray();
	return {SettingsArray[1], SettingsArray[2], SettingsArray[3]};
}

std::string ReturnShowKeysValue(){
	if(SettingsArray[4] == "") InitializeSettingsArray();
	return SettingsArray[4];
}

std::string ReturnInspectionTimeValue(){
	if(SettingsArray[5] == "") InitializeSettingsArray();
	return SettingsArray[5];
}

int ReturnChosenOption(){
	return ChosenOption;
}
