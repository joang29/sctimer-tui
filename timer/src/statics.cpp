#include<string>
#include<array>
#include<iomanip>
#include<vector>
#include<numeric>
#include<fstream>

std::array<double,4> SumAverage();
std::array<std::string,4> DetermineStatics();
float NumberToTime(float);

std::array<std::string, 4> DetermineStatics(){
	std::ifstream StaticsFile("timer/config/statics.txt");
	
	if(!StaticsFile.is_open()){
		std::ofstream WriteStaticsFile("timer/config/statics.txt", std::ofstream::app);
		
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
	std::ifstream TimesFile("timer/config/times.txt");

	if(!TimesFile.is_open()){
		std::ofstream {"timer/config/times.txt"};
		TimesFile.close();
		
		return {0.0,0.0,0.0,0.0};
	}
	
	std::string line = "";
	std::vector <double> average;
	int NumberLine = 0;

	while(TimesFile.eof()==0){
		getline(TimesFile, line);
		if(line != ""){
			if(line.find(":") != std::string::npos) line.replace(line.find(":"), 1, ""); 
			average.push_back(stod(line));
		}
		NumberLine++;
	}
	
	double sumAo5 = 0, sumAo12 = 0, sumAo50 = 0;

	if(NumberLine > 5) sumAo5 = std::accumulate(average.begin()+NumberLine-6, average.end(), 0.0)/5;
	else return {0, 0, 0, NumberLine*1.0};
	
	if(NumberLine > 12) sumAo12 = std::accumulate(average.begin()+NumberLine-13, average.end(), 0.0)/12;
	else return {sumAo5, 0, 0, NumberLine*1.0}; 
	
	if(NumberLine > 50) sumAo50 = std::accumulate(average.begin()+NumberLine-51, average.end(), 0.0)/50;
	else return {sumAo5, sumAo12, 0, NumberLine*1.0};
	
	return {sumAo5, sumAo12, sumAo50, NumberLine*1.0};
}

void ChangePB(std::array<std::string,4> BestTimes){
	std::ofstream NewFile("timer/config/NewFileStatics.conf");
	
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
		
	std::rename("timer/config/NewFileStatics.conf", "timer/config/statics.txt");

	NewFile.close();
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

	}else if(average >= 100 && stof(std::to_string(average).substr(1,std::to_string(average).find(":"))) >= 60){
		average += 100;
		average -= 60;
	}
	return average;
}
