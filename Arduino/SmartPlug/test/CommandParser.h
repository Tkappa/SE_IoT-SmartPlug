#ifndef __COMMPARSER__
#define __COMMPARSER__

#define NUMBERSTOPARSE 10
#define COMMANDMAXSIZE 6
#define NUMBERMAXPARAMS 4
#define PARAMSMAXSIZE 20

#define DEBUG

#include "Arduino.h"

enum ParseMode{PM_search,PM_params,PM_checkend};

class CommandParser{
  private:
  char commandsToParse[NUMBERSTOPARSE][COMMANDMAXSIZE];
  int currentPositions[NUMBERSTOPARSE];
  char parameters[NUMBERMAXPARAMS][PARAMSMAXSIZE];

  ParseMode currentMode;

  String wordToFind;
  int nCommands;
  int nParams;
  int commandFound;
  int currentParam;
  int currentParamLen;

public:
  CommandParser(){
    for(int i =0; i<NUMBERSTOPARSE;i++){
      strcpy(commandsToParse[i],'\0');
      currentPositions[i]=0;
    }
    nCommands=0;
    reset();
  }

  void reset(){
    for(int i =0; i<NUMBERMAXPARAMS;i++){
      strcpy(parameters[i],'\0');
    }
    currentMode=PM_search;
    nParams=0;
    commandFound=-1;
    currentParam=0;
    currentParamLen=0;
  }

  String getParam(int n){
    return parameters[n];
  }

  bool addCommand(const char* param){
    if (nCommands<NUMBERSTOPARSE){
      strcpy(commandsToParse[nCommands],param);
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
          #ifdef DEBUG
            Serial.print(commandsToParse[i][currentPositions[i]]);
            Serial.print("=");
            Serial.println(c);
          #endif
          if(commandsToParse[i][currentPositions[i]]==c){
            if(c=='='){
              commandFound=i;
              currentParam=0;
              currentMode=PM_params;
              #ifdef DEBUG
                Serial.print(F("Command found - "));
                Serial.println(commandsToParse[commandFound]);
              #endif
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
          char cat = (char)c;
          parameters[currentParam][currentParamLen]=c;
          currentParamLen++;
        }
        else{
          parameters[currentParam][currentParamLen]='\0';
          currentParamLen=0;
          #ifdef DEBUG
            Serial.print(F("Param "));
            Serial.print(currentParam);
            Serial.print(F(" found - "));
            Serial.println(parameters[currentParam]);
          #endif
          currentParam++;
          currentMode=PM_checkend;
        }
      break;
      case PM_checkend:
        if(c=='-'){
          #ifdef DEBUG
            Serial.println(F("Finished parsing"));
          #endif
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
