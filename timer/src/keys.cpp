#ifdef _WIN32
	#include<conio.h>
#endif
#include<string>
#include "functions.h"

void keys(){	
	while(true){
		#ifdef _WIN32 
			char KeyPressed = getch();
		#elif __linux__
			char KeyPressed = getchar();
		#endif

		switch(KeyPressed){
			case 'q':
			case 'Q': if(!FunctionTimerIsOn()) interface("timer");
			break;

			case 'e':
			case 'E': if(!FunctionTimerIsOn()) interface("statics");
			break;

			case 'r':
			case 'R': if(!FunctionTimerIsOn()) interface("settings");
			break;
			
			case 'x':
			case 'X': 
				  #ifdef __linux__ 
				  		system("stty cooked | clear"); 
				  #elif _WIN32
				  		system("cls");
				  #endif
				  exit(0);
			break;
			
			case ' ': if(ReturnActualInterface() == "timer"){
					StartTimer();
				  }else if(ReturnActualInterface() == "settings"){
					ChangeValueOfSettings();
					interface("settings");
				}
			break;
			
			case 'w':
			case 'W': if(ReturnActualInterface() == "settings"){
					ChangeChosenOption('-');	
				  	interface("settings");
				  }
			break;

			case 's':
			case 'S': if(ReturnActualInterface() == "settings"){
					ChangeChosenOption('+');
					interface("settings");
				  }
			break;
			
			case 'h':
			case 'H': if(ReturnScreenAfterSolve()) options("save");
			break;

			case 'f':
			case 'F': if(ReturnScreenAfterSolve()) options("DNF");
			break;

			case 'g':
		    	case 'G': if(ReturnScreenAfterSolve()) options("+2");
			break;
		}
	}
}
