#ifndef __WORDFINDER__
#define __WORDFINDER__

#define DEBUG

#include "Arduino.h"

class WordFinder{

public:
  WordFinder(){ }

  void setWord(char *  toFind){
    Serial.print("AAAA:");
    Serial.println(toFind);
    wordToFind=toFind;
    currAt=0;
    Serial.print("SetWord:");
    Serial.println(wordToFind);
  }

  bool search(char c){
    #ifdef DEBUG
    Serial.print(c);
    Serial.print("=");
    Serial.println(wordToFind[currAt]);
    #endif
    if(c==wordToFind[currAt]){;
      currAt++;
      if(wordToFind[currAt]=='\0'){
        return true;
      }
    }
    else{
      currAt=0;
    }
    return false;
  }

private:
  char * wordToFind;
  int currAt;
};

#endif
