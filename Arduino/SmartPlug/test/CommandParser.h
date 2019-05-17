#ifndef __COMMPARSER__
#define __COMMPARSER__

#define NUMBERSTOPARSE 10

#include "Arduino.h"

enum ParseMode{PM_search,PM_params,PM_checkend};

class CommandParser{
  private:
  char* commandsToParse[NUMBERSTOPARSE];
  int currentPositions[NUMBERSTOPARSE];
  char* parameters[NUMBERSTOPARSE];

  ParseMode currentMode;

  String wordToFind;
  int nCommands;
  int nParams;
  int commandFound;
  int currentParam;

public:
  CommandParser(){
    for(int i =0; i<NUMBERSTOPARSE;i++){
      strcpy(commandsToParse[i],"");
    }
    nCommands=0;
    reset();
  }

  void reset(){
    for(int i =0; i<NUMBERSTOPARSE;i++){
      strcpy(parameters[i],"");
      currentPositions[i]=0;
    }
    currentMode=PM_search;
    nParams=0;
    commandFound=-1;
    currentParam=0;
  }

  String getParam(int n){
    return parameters[n];
  }

  bool addCommand(char* param){
    if (nCommands<NUMBERSTOPARSE){
      commandsToParse[nCommands]=param;
      currentPositions[nCommands]=0;
      nCommands++;
      return true;
    }
    else{
      return false;
    }
  }

  int parse(char c){
    switch(currentMode){
      case PM_search:
        for(int i=0;i<nCommands;i++){
          //Serial.print(commandsToParse[i][currentPositions[i]]);
          //Serial.print("=");
          //Serial.println(c);
          if(commandsToParse[i][currentPositions[i]]==c){
            if(c=='='){
              commandFound=i;
              currentParam=0;
              currentMode=PM_params;
              Serial.print(F("Command found - "));
              Serial.println(commandsToParse[commandFound]);
            }
            else{
              currentPositions[i]=currentPositions[i]+1;
            }
          }
          else{
            currentPositions[i]=0;
          }
        }
      break;
      case PM_params:
        if(c!=';'){
          strcat(parameters[currentParam],&c);
          //parameters[currentParam].concat(c);
        }
        else{
          Serial.print(F("Param "));
          Serial.print(currentParam);
          Serial.print(F(" found - "));
          Serial.println(parameters[currentParam]);
          currentParam++;
          currentMode=PM_checkend;
        }
      break;
      case PM_checkend:
        if(c=='-'){
          Serial.println(F("Finished parsing"));
          return commandFound;
        }
        else{
          currentMode=PM_params;
          parse(c);
        }
      break;
    }
    return -1;
  }
};

#endif
