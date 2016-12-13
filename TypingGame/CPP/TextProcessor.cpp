#include <ctype.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
#include "../Header/TextProcessor.h"

// Returns a queue with strings and spaces in between each string
queue<string> TextProcessor::split(char *arr, char delim) {
  queue<string> buff;
  string str;
  for (int i = 0; arr[i] != '\0'; i++) {

    if (isspace(arr[i])) {
      buff.push(str);
      str = "";
      str.clear();
      buff.push(string(" "));
    } else {
      str += arr[i];
    }

    if (arr[i+1] == '\0') {
      buff.push(str);
      str = "";
      str.clear();
    }
  }
  return buff;
}

// Clean up the buffer for printing
char* TextProcessor::formatBuffer(char *fb, int fbLength) {
  // TODO encapsulate int length and initialization
  int cblength = 0; // clean buffer length

  for (int i = 0; i < fbLength; i++) {
    if (isprint(fb[i]) || isspace(fb[i])) cblength++; // get number of printable chars
  }

  char *cleanBuffer = new char[cblength + 1];
  int k = 0;

  for (int i = 0; i < fbLength; i++) {
    if (isprint(fb[i])) { // || isspace(fb[i]
      cleanBuffer[k] =  fb[i]; // put printable chars in a clean buffer
      k++;
    }
    if (k > 0 && !isspace(cleanBuffer[k-1]) && fb[i] == '\n') {
      cleanBuffer[k] = ' ';
      k++;
    }
  }
  cleanBuffer[k + 1] = '\0';
  return cleanBuffer;
}

TextProcessor::TextProcessor() {
  cout << "TextProcessor default constructor" << endl;
}

TextProcessor::TextProcessor(const char *file) {
  cout << "TextProcessor one param constructor" << endl;
  loadFile(file);
}

TextProcessor::~TextProcessor() {
  cout << "TextProcessor destructor" << endl;
  delete[] buffer;
}

void TextProcessor::loadFile(const char *file) {
  ifstream infile(file, ifstream::in | ifstream::binary); // read file
  //stringstream ss;

  if (infile.is_open()) {
    //TODO Convert file text to a common text (unicode or something); unique font = bad
    //TODO handle Japanese/Chinese/more text/input?

    infile.seekg(0, infile.end);
    int rawLength = infile.tellg(); // total number of characters in file
    infile.seekg(0, infile.beg);

    char *rawBuffer = new char[rawLength];
    infile.read(rawBuffer, rawLength); // read raw data as character block
    buffer = formatBuffer(rawBuffer, rawLength);
    delete[] rawBuffer;
  }
  storage = split(buffer, ' ');
  // printAll();
  infile.close();
  cout << "Text file loaded" << endl;
  //printAll();
}

char* TextProcessor::getBuffer() {
  return buffer;
}

void TextProcessor::printAll() {
  cout << endl << "Buffer" << endl;
  for (int i = 0; buffer[i] != '\0'; i++) {
    cout << buffer[i];
  }
  cout << endl;

  cout << endl << "Storage" << endl;
  for (int i = 0; i < storage.size(); i++) {
    cout << storage.front();
    storage.push(storage.front());
    storage.pop();
  }
  cout << endl;
}
