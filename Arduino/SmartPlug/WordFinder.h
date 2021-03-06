#ifndef __WORDFINDER__
#define __WORDFINDER__

//#define WPDEBUG

#include "Arduino.h"

class WordFinder{

public:
  WordFinder(){ }

  void setWord(char *  toFind){
    wordToFind=toFind;
    currAt=0;
    #ifdef WPDEBUG
      Serial.print("Word to find:");
      Serial.println(wordToFind);
    #endif
  }

  bool search(char c){
    #ifdef WPDEBUG
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
