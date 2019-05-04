#ifndef __WORDFINDER__
#define __WORDFINDER__


#include "Arduino.h"

class WordFinder{

public:
  WordFinder(String toFind){
    wordToFind=toFind;
    currAt=0;
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
  String wordToFind;
  int currAt;
};

#endif
