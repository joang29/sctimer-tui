#include "functions.h"
#include<algorithm>
#include<iterator>
#include<string>
#include<array>
#include<iomanip>
#include<vector>
#include<stdio.h>
#include<numeric>
#include<fstream>

#ifdef _WIN32
	std::string ConfigDirectoryPath = std::string(getenv("APPDATA")) + "/../Local/sctimer-cli/";
#elif __linux__
	std::string ConfigDirectoryPath = std::string(getenv("HOME")) + "/.config/sctimer-cli/";
#endif

std::array<double,4> SumAverage();
std::array<std::string,4> DetermineStatics();
float NumberToTime(float);

std::array<std::string, 4> DetermineStatics(){
	std::string cube = ReturnCube() == "3x3" ? "3x3/statics_3x3.txt" : "2x2/statics_2x2.txt";

	std::ifstream StaticsFile(ConfigDirectoryPath + cube);
	
	if(!StaticsFile.is_open()){
		std::ofstream WriteStaticsFile(ConfigDirectoryPath + cube, std::ofstream::app);
		
		WriteStaticsFile<<"PB =          --\nBestAo5 =     --\nBestAo12 =    --\nBestAo50 =    --\n";
		WriteStaticsFile.close();
		StaticsFile.close();
		
		return {"--", "--", "--", "--"};
	}
	
	std::string line = "";
	std::array<std::string,4> statics;
	int NumberLine = 0;
	
	while(StaticsFile.eof()==0 && NumberLine < 4){
		getline(StaticsFile, line);
		if(line != "") statics[NumberLine] = line.substr(14);
		NumberLine++;
	}

	return statics;
}

std::array<double,4> SumAverage(){
	
	std::string cube = ReturnCube() == "3x3" ? "3x3/times_3x3.txt" : "2x2/times_2x2.txt";

	std::ifstream TimesFile(ConfigDirectoryPath + cube);

	if(!TimesFile.is_open()){
		std::ofstream {ConfigDirectoryPath + cube};
		TimesFile.close();
		
		return {0.0,0.0,0.0,0.0};
	}
	
	std::string line = "";
	std::vector <double> average;
	std::vector <int> SolveDNF;
	int NumberLine = 0;

	while(TimesFile.eof()==0){
		getline(TimesFile, line);
		if(line != ""){
			if(line.find(":") != std::string::npos) line.replace(line.find(":"), 1, ""); 
			if(line == "DNF") SolveDNF.push_back(NumberLine);
			else average.push_back(stod(line));
		}
		NumberLine++;
	}

	double sumAo5 = 0, sumAo12 = 0, sumAo50 = 0;
	int NumberOfDNFao5 = 0, NumberOfDNFao12 = 0, NumberOfDNFao50 = 0;
	
	if(SolveDNF.size() != 0){
		for(int line : SolveDNF){
			if(line > NumberLine-6){
				NumberOfDNFao5++;
				NumberOfDNFao12++;
				NumberOfDNFao50++;
			}else if(line > NumberLine-13){
				NumberOfDNFao12++;
				NumberOfDNFao50++;
			}else if(line > NumberLine-51) NumberOfDNFao50++;
		}
	}

	if(NumberLine > 5){
		if(NumberOfDNFao5 == 1){ 	
			std::vector <double>::iterator BestSolve = std::min_element(average.end()-5, average.end());
			sumAo5 = (std::accumulate(average.end()-4, average.end(), 0.0) - *BestSolve)/3;
		
		}else if(NumberOfDNFao5 >= 2) sumAo5 = -1;
		
		else sumAo5 = std::accumulate(average.end()-5, average.end(), 0.0)/5;
	
	}else return {0, 0, 0, NumberLine*1.0};
	
	if(NumberLine > 12){
		if(NumberOfDNFao12 == 1){ 		
			std::vector <double>::iterator BestSolve = std::min_element(average.end()-12, average.end());
			sumAo12 = (std::accumulate(average.end()-11, average.end(), 0.0) - *BestSolve)/10;
		
		}else if(NumberOfDNFao12 >= 2) sumAo12 = -1;
		
		else sumAo12 = std::accumulate(average.end()-12, average.end(), 0.0)/12;
	
	}else return {sumAo5, 0, 0, NumberLine*1.0}; 
	
	if(NumberLine > 50){
		if(NumberOfDNFao50 > 0 && NumberOfDNFao50 < 4){ 
			std::vector <std::vector<double>::iterator> BestSolves;
			
			for(int i = 0; i < NumberOfDNFao50; i++){
				BestSolves.push_back(std::min_element(average.begin()+NumberLine-49-i, average.end()));
				average.erase(std::find(average.begin(), average.end(), *BestSolves[i]));
			}
			
			if(NumberLine >= NumberOfDNFao50 + 50) sumAo50 = std::accumulate(average.begin()+NumberLine-49-NumberOfDNFao50, average.end(), 0.0);
			else if(NumberLine == 51) sumAo50 = std::accumulate(average.begin(), average.end(), 0.0);
			else if(NumberLine == 52) sumAo50 = std::accumulate(average.begin()+1, average.end(), 0.0);
			
			sumAo50 /= (50-NumberOfDNFao50*2);
	
		}else if(NumberOfDNFao50 >= 4) sumAo50 = -1;
		
		else sumAo50 = std::accumulate(average.end()-50, average.end(), 0.0)/50;
	}
	
	return {sumAo5, sumAo12, sumAo50, NumberLine*1.0};
}

void ChangePB(std::array<std::string,4> BestTimes){
	std::ofstream NewFile(ConfigDirectoryPath +"NewFileStatics.txt");
	
	for(int i = 0; i<4; i++){
		if(BestTimes[i] != "--" && BestTimes[i] != "0.000000"){
			float time = NumberToTime(stof(BestTimes[i]));

			std::stringstream ss;
			ss<<std::fixed<<std::setprecision(2)<<time;
			BestTimes[i] = ss.str();
			
			if(stof(BestTimes[i]) >= 1000) BestTimes[i].insert(2, ":");		
			else if(stof(BestTimes[i]) >= 100) BestTimes[i].insert(1, ":");
		}else BestTimes[i] = "--";
	}
	
	NewFile<<"PB =          "<<BestTimes[0]<<"\n";
	NewFile<<"BestAo5 =     "<<BestTimes[1]<<"\n";
	NewFile<<"BestAo12 =    "<<BestTimes[2]<<"\n";
	NewFile<<"BestAo50 =    "<<BestTimes[3]<<"\n";
	
	NewFile.close();
	
	std::string cube = ReturnCube() == "3x3" ? "3x3/statics_3x3.txt" : "2x2/statics_2x2.txt";

	#ifdef _WIN32
		remove((ConfigDirectoryPath + cube).c_str());
		rename((ConfigDirectoryPath + "NewFileStatics.txt").c_str(), (ConfigDirectoryPath + cube).c_str());
	#elif __linux__
		rename((ConfigDirectoryPath + "NewFileStatics.txt").c_str(), (ConfigDirectoryPath + cube).c_str());
		#endif
}

float NumberToTime(float average){
	if(stof(std::to_string(average).substr(std::to_string(average).find("."))) >= 60){
		average += 1;
		average -= 0.60;
	}
	if(average >= 60 && average < 100){
		average += 100;
		average -= 60;
	}
	if(average >= 1000 && stof(std::to_string(average).substr(2, std::to_string(average).find("."))) >= 60){
		average += 100;
		average -= 60;	

	}else if(average >= 100 && stof(std::to_string(average).substr(1,2)) >= 60){
		average += 100;
		average -= 60;
	}
	return average;
}
