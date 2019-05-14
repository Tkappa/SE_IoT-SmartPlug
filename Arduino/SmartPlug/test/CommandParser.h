#ifndef __COMMPARSER__
#define __COMMPARSER__

#define NUMBERSTOPARSE 10

#include "Arduino.h"

enum ParseMode{PM_search,PM_params,PM_checkend};

class CommandParser{
  private:
  String commandsToParse[NUMBERSTOPARSE];
  int currentPositions[NUMBERSTOPARSE];
  String parameters[NUMBERSTOPARSE];

  ParseMode currentMode;

  String wordToFind;
  int nCommands;
  int nParams;
  int commandFound;
  int currentParam;

public:
  CommandParser(){
    for(int i =0; i<NUMBERSTOPARSE;i++){
      commandsToParse[i]= String("");
    }
    nCommands=0;
    reset();
  }

  void reset(){
    for(int i =0; i<NUMBERSTOPARSE;i++){
      parameters[i]= String("");
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

  bool addCommand(String param){
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
              Serial.print("Command found - ");
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
          parameters[currentParam].concat(c);
        }
        else{
          Serial.print("Param ");
          Serial.print(currentParam);
          Serial.print(" found - ");
          Serial.println(parameters[currentParam]);
          currentParam++;
          currentMode=PM_checkend;
        }
      break;
      case PM_checkend:
        if(c=='-'){
          Serial.println("Finished parsing");
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
