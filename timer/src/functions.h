#ifndef FUNCTIONS
#define FUNCTIONS
#include<string>
#include<array>

//timer.cpp
void StartTimer();
void ScrambleGenerator();
bool FunctionTimerIsOn();

//keys.cpp
void keys();

//interface.cpp
void interface(std::string);
std::string ReturnActualInterface();

//settings.cpp
void ChangeValueOfSettings();
void ChangeChosenOption(char);
int ReturnChosenOption();
std::string ReturnShowKeysValue();
std::string ReturnInspectionTimeValue();
std::array<std::string,5> InitializeSettingsArray();
std::array<std::string, 3> ReturnColors();


//statics.cpp
std::array<double,4> SumAverage();
std::array<std::string,4> DetermineStatics();
void ChangePB(std::array<std::string,4>);
float NumberToTime(float);

#endif
