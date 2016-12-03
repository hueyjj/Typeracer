// TextProcessor.h

#ifndef _TEXT_PROCESSOR_H_INCLUDE_

#define _TEXT_PROCESSOR_H_

#include <ctype.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

/* Processes the text and holds all the information for the next words */
class TextProcessor{
 private:
  char *buffer;

  //int length;
  char* formatBuffer(char *fb, int fbLength);
  queue<string> split(char *arr, char delim);
  //char delim;
  //vector<string> storage;
  //void splitLine(stringstream &ss, char dl);
  //void addWord(string *str);
 public:
  queue<string> storage;
  TextProcessor();
  TextProcessor(const char *file);
  ~TextProcessor();
  void loadFile(const char *file);
  char* getBuffer();
  void printAll();
};

#endif
